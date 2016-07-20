#include "dmfgui.h"
#include "ui_dmfgui.h"
#include <QtSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QtWidgets>
#include <QtGui>
#include <QtCore>
#include <iostream>
#include <QSignalMapper>
#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <cstdlib>
#include <climits>
#include <QCloseEvent>
#include <QFile>
#include <time.h>
#include <algorithm>
#include <QStack>
#include "node.h"


QPushButton **dmf_array;
QGridLayout *gridLayout;
QSignalMapper *mapper;

//used for numbering the electrodes
int numberingcount = 0;                          //electrode numbering
int reservoircount = 0;                          //reservoir numbering
int newrow,newcolumn,resnum,corner;              //newrow, newcolumn = for total rows and columns (including empty spaces)
                                                 //resnum = number of reservoirs to be added
QString row,column;                              //Temporairly global variables

int added = 0;                                   //keeping track of how many reservoirs were added

int elec = 1;                                    //Start by adding values to the first electrode

QString to_Send = "";                            //Information that gets passed to the Arduino
bool enter_Button_Clicked =false;                //ensures that user does not press the enter Button twice


//for nemesys
double flowRate;
double volume;
bool opened = false;
bool dosing = false;

bool addRes = false;                            //true if need to keep adding reservoirs.
                                                //false if all reservoirs were added

bool mapCreated = false;                        //True if the map (Grid) has been created

bool realTime = false;                          //If true, turn on electrodes one at a time in real-time

QStringList ordCP;                              //This contains the Contact PAD Information from DIPTRACE

bool deleteButtonchoice = false;                //Keeps track if delete button is enabled

bool defaultNumbering = false;                  //Keeps track if the map should be loaded with default contact pad numbering

bool newWindowButtonpressed = false;            //True if newLayout was selected to generate the map

bool loadWindowButtonpressed = false;           //True if loadLayout was selected to generate the map

QList<int> Coordz;                              //Stores the layout information when user saves their layout

int x;
int y;

boolean autoGen = false;            //for clearing purposes

//dimensions
int horizontalSize;
int verticalSize;
int avail;
int *eavail;



/*
 * Structure of an electrode, array of 2 electrodes and stores their coordinates
 */
struct coordinates
{
    int x;
    int y;
}electrode_1,electrode_2;

/*--------------------------------------------------------------------------------------------------------
 *
 * Setting up GUI and basic button functions
 * #TODO:Monitoring progress of the execution of the commands
 *       Implementing feedback system
 *
--------------------------------------------------------------------------------------------------------*/

DMFgui::DMFgui(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DMFgui)
{
    ui->setupUi(this);

    arduino_is_available = false;
    arduino_port_name = "";

    ui->textEdit->setReadOnly(true); //not allowing user to change anything

    //for Nemesys
    ui->targetVolumeEdit->setReadOnly(true);
    ui->targetFlowRateEdit->setReadOnly(true);
    ui->unitsComboBox->addItem("nL");
    ui->unitsComboBox->addItem("μL");
    ui->unitsComboBox->addItem("mL");
    ui->unitsComboBox->addItem("L");
    ui->unitsComboBox->addItem("mm");
    ui->unitsComboBox->setCurrentIndex(1);

    ui->funitscomboBox->addItem("nL/s");
    ui->funitscomboBox->addItem("nL/min");
    ui->funitscomboBox->addItem("μL/s");
    ui->funitscomboBox->addItem("μL/min");
    ui->funitscomboBox->addItem("μL/h");
    ui->funitscomboBox->addItem("mL/min");
    ui->funitscomboBox->addItem("mL/h");
    ui->funitscomboBox->addItem("mm/s");
    ui->funitscomboBox->setCurrentIndex(2);

    ui->dosingUnitscomboBox->addItem("1");
    ui->dosingUnitscomboBox->addItem("2");
    ui->dosingUnitscomboBox->addItem("3");

    //defining arduino here
    arduino=new QSerialPort;

    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
    {
        if (serialPortInfo.hasVendorIdentifier()&& serialPortInfo.hasProductIdentifier())
        {
            if (serialPortInfo.vendorIdentifier() == arduino_uno_vendor_id)
            {
                if (serialPortInfo.productIdentifier()==arduino_uno_product_id)
                {
                    arduino_port_name = serialPortInfo.portName();
                    arduino_is_available = true;
                }
            }
        }
    }
    if (arduino_is_available){
        arduino->setPortName(arduino_port_name);            //open and configure the serialport
        arduino->open(QSerialPort::ReadWrite);              //open it, Read or Write capabilities
        arduino->setBaudRate(QSerialPort::Baud9600);        //make sure that Baud rate matches what's on the arduino
        arduino->setDataBits(QSerialPort::Data8);
        arduino->setParity(QSerialPort::NoParity);
        arduino->setStopBits(QSerialPort::OneStop);
        arduino->setFlowControl(QSerialPort::NoFlowControl);
    }
    else{                                                   //give an error message, (no arduino available)
         QMessageBox::warning(this,"Port error", "Couldn't find the Arduino!");
    }
}

DMFgui::~DMFgui()
{
    if (arduino->isOpen())
    {
        arduino->close();
    }
    delete ui;
}

/*
 * "x" button on the main window
 */
void DMFgui::closeEvent(QCloseEvent *event){
    if (opened){                                            //close nemesys if it was opened
        nemesys->closeConnection();
    }
    event -> accept();                                      //allows the main window to close
}

/*
 * Information that gets sent to Arduino
 */

void DMFgui::save_to_String(QString electrode_num){                 //#LOOK INTO STRINGSTREAM
    to_Send += electrode_num + ",";
    if (electrode_num=="1000" || electrode_num=="1001"|| electrode_num=="1003"){
        ui->textEdit->insertPlainText("\n"+electrode_num + ",");    //special commands.
    }
    else{
        ui->textEdit->insertPlainText(""+electrode_num + ",");      //normal commands
    }
}
/*
 * Undo previous information stored in to_Send
 */
void DMFgui::on_UndoButton_clicked(){
    ui->textEdit->undo();                                   //find a way to delete one by one.
}

void DMFgui::on_mixButton_clicked(){
    save_to_String("1003");
}
void DMFgui::on_sequenceButton_clicked(){
    save_to_String("1000");
}
void DMFgui::on_splitButton_clicked(){
    save_to_String("1001");
}
/*
 * Closes main window, serves the same purpose as "x" button
 * #Is this even useful?
 */
void DMFgui::on_exitButton_clicked(){
    QApplication::quit();
}
/*--------------------------------------------------------------------------------------------------------
 *
 * Arduino related Function
 * #TODO: Look into StringStream in order to change the values sent to arduino
 *
--------------------------------------------------------------------------------------------------------*/
/*
 * Reset information being sent to Arduino
 */
void DMFgui::on_resetButton_clicked(){
    to_Send = "";
    ui->textEdit->clear();
}
/*
 * Sending information to Arduino
 */
void DMFgui::on_sendButton_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,"Are you sure","Are you sure?",QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes){                             //send information if user selects "Yes"
        ui->textEdit->insertPlainText("\nSending...");

        updateDMF(to_Send);                                     //Sending information to arduino

        ui->textEdit->insertPlainText("\nSent!");
        //ui->textEdit->insertPlainText(to_Send);
    }                                                           //automatically go back if user is not sure
}
/*
 * Communicating information to Arduino
 *  Write to Arduino in blocks of X-number of electrodes
 */

void DMFgui::updateDMF(QString to_Send)
{
    QStringList sep = to_Send.split(",");                       //Stores String being sent into a List
    double ElectrodesPerBlock = 5;                              //Sets the number of electrodes sent per block
    int actualLength = sep.length()-1;
    int rem = actualLength%(int)ElectrodesPerBlock;
    ui->textEdit->insertPlainText("rem: " + QString::number(rem)+"\n");
    ui->textEdit->insertPlainText("length: " + QString::number(actualLength)+"\n");
    ui->textEdit->insertPlainText("ceil: " + QString::number(ceil(actualLength/ElectrodesPerBlock))+"\n");
    int b = (int)ceil(actualLength/ElectrodesPerBlock);         //Determine how many blocks will be sent

    ui->textEdit->insertPlainText("convceil: " + QString::number(b)+"\n");
    QString *sep20 = new QString[b];
    int q;
    /*--------
     * The following code manipulates the Information being sent to ensure that the final element doesn't contain a ","
     * ---------*/
    if(rem==0){
        q=b;
    }
    else
        q=b-1;
    for(int i=0;i<q;i++){                                       //Create strings up until the very last one which may contain less than ElectrodesPerBlock electrodes
        for(int j=(i*ElectrodesPerBlock);j<(i*ElectrodesPerBlock)+(ElectrodesPerBlock-1);j++){
            sep20[i]=sep20[i].append(sep.at(j)+",");
        }
        sep20[i]=sep20[i].append(sep.at((i*ElectrodesPerBlock)+(ElectrodesPerBlock-1)));
    }
    if(rem!=0){
    for(int k=((b-1)*ElectrodesPerBlock);k<((b-1)*ElectrodesPerBlock)+(rem-1);k++){
        sep20[b-1]=sep20[b-1].append(sep.at(k)+",");
    }
    sep20[b-1]=sep20[b-1].append(sep.at((b-1)*(ElectrodesPerBlock)+(rem-1)));
    }//ui->textEdit->setPlainText("");
    for (int k=0;k<b;k++){
        ui->textEdit->insertPlainText(sep20[k] + "\n");
    }

    if (arduino->isWritable())                                  //if Aruidno is available
    {
        QByteArray readData = arduino->readAll();

        QByteArray v = "2";                                     //Detection value, arduino will have to send this back as a signal for us to send the next block of electrodes

        QString s_data = readData;
        ui->textEdit->insertPlainText("data: " + s_data);
        for(int i = 0; i<b;i++){
            arduino->write(sep20[i].toStdString().c_str());     //communicating with Arduino
            while(readData!=v){
                readData = arduino->readAll();
                s_data = readData;
                ui->textEdit->insertPlainText(s_data);
                qApp->processEvents();
            }
            readData = arduino->readAll();
             ui->textEdit->insertPlainText("\ni: " + QString::number(i));
        }
    }
    else
    {
        qDebug() << "Couldn't write to serial";
    }
}

/*--------------------------------------------------------------------------------------------------------
 *
 * Setting up configuration of DMF device
 *
--------------------------------------------------------------------------------------------------------*/
/*
 * "main" function for controlling adding of resevoirs
 */
void DMFgui::on_enterButton_clicked()
{

    QString row;
    QString column;
    QList <int> CreateMapInfoRow;                                   //Holds the row coordinates from the loaded layout
    QList <int> CreateMapInfoCol;                                   //Holds the column coordinates from the loaded layout


    if (newWindowButtonpressed){                                    //Setting the parameters for creating the grid if new layout is selected
        row = nLayout->savedRow;
        column = nLayout->savedRow;
        newrow = row.toDouble()+4;
        newcolumn = column.toDouble()+4;
        horizontalSize = newcolumn;
        verticalSize = newrow;
        ui->textEdit->insertPlainText("parameters are set up");
    }
    else if(loadWindowButtonpressed)                                //Load layout was selected
    {
        QString fileName = lLayout->savedLayoutfilename;
        defaultNumbering = lLayout->savedNumbering;

        QFile file (fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))      //Opens up layout information file
            return;
        QTextStream in(&file);
        QStringList mazeInfo;
        while (!in.atEnd()) {
             mazeInfo.append(in.readLine());                        //Saves information in a List
        }

        QStringList tempMap;
        for(int a=0;a<mazeInfo.length();a++){
            tempMap = mazeInfo.at(a).split(",");
            CreateMapInfoRow.append(tempMap.at(0).toInt());
            CreateMapInfoCol.append(tempMap.at(1).toInt());
        }

        int maprow = CreateMapInfoRow.at(0);                         //The first two numbers of the layout file contain the grid size
        int mapcol = CreateMapInfoCol.at(0);
        newrow = maprow;
        newcolumn = mapcol;

    }
    else{
        QMessageBox::warning(this,tr("No Layout has been selected"), tr("Please select a layout\nEither New Layout or Load Layout"));
    }

    if(newWindowButtonpressed ||loadWindowButtonpressed ){
        gridLayout = new QGridLayout;                               //initializing new GridLayout
        mapper = new QSignalMapper;
        QLabel display;

        gridLayout->setHorizontalSpacing(0);                        //set up of Layout
        gridLayout->setVerticalSpacing(0);
        gridLayout->setSpacing(0);



        if(!enter_Button_Clicked)
        {
            dmf_array = new QPushButton*[newrow];                   //dynamically creating array for button storage
            for(int i=0;i<newrow;i++){
                dmf_array[i]=new QPushButton[newcolumn];
            }

            QLabel *empty = new QLabel(this);
            int s=1;


            for (int i=0;i<newrow;i++)                              //naming and setting properties for each button
            {

                for (int j=0;j<newcolumn;j++)
                {
                    if(newWindowButtonpressed){                     //New layout was chosen
                        int drow = row.toDouble();
                        int dcolumn = column.toDouble();
                        if (i==0||i==1||j==0||j==1||i==drow+3||i==drow+2||j==dcolumn+3||j==dcolumn+2)
                        {                                          //Create empty spaces to allow room for reservoirs
                            gridLayout->addWidget(empty,i,j);
                        }

                        else                                        //adding a new button
                        {
                            numberingcount++;
                            dmf_array[i][j].setStyleSheet( "border-style: outset ;border-width: 2px; border-color: grey");
                            gridLayout->addWidget(&dmf_array[i][j],i,j);

                            if(defaultNumbering)                    //Set the numbering of the electrodes in ascending order
                            {
                                mapper->connect(&dmf_array[i][j],SIGNAL(clicked()),mapper,SLOT(map()));
                                dmf_array[i][j].setText(QString::number(numberingcount));
                                mapper->setMapping(&dmf_array[i][j],QString::number(i)+","+QString::number(j)+","+QString::number(numberingcount));

                            }
                            else                                    //Set the numbering of the electrodes as a non-empty value, in this case "_" is chosen
                            {
                                mapper->connect(&dmf_array[i][j],SIGNAL(clicked()),mapper,SLOT(map()));
                                dmf_array[i][j].setText("_");
                                mapper->setMapping(&dmf_array[i][j],QString::number(i)+","+QString::number(j)+","+"_");
                            }
                        }
                    }
                    else                                            //Load layout was chosen
                    {
                        if (s<CreateMapInfoCol.length() && i==CreateMapInfoRow.at(s) && j==CreateMapInfoCol.at(s))
                        {                                           //Create buttons that respect the coordinates given in the load layout file
                            dmf_array[i][j].setStyleSheet("border-style: outset ;border-width: 2px; border-color: grey");
                            gridLayout->addWidget(&dmf_array[i][j],i,j);

                            if(defaultNumbering)                    //Set the numbering of the electrodes in ascending order
                            {
                                dmf_array[i][j].setText(QString::number(s));
                                mapper->setMapping(&dmf_array[i][j],QString::number(i)+","+QString::number(j)+","+QString::number(s));
                                mapper->connect(&dmf_array[i][j],SIGNAL(clicked()),mapper,SLOT(map()));
                            }
                            else                                    //Set the numbering of the electrodes as a non-empty value, in this case "_" is chosen
                            {
                                dmf_array[i][j].setText("_");
                                mapper->setMapping(&dmf_array[i][j],QString::number(i)+","+QString::number(j)+","+"_");

                            }
                            s++;
                        }
                        else
                        {
                            gridLayout->addWidget(empty,i,j);
                        }
                    }
                }
            }


            if(defaultNumbering || (newWindowButtonpressed )){
                connect(mapper,SIGNAL(mapped(QString)),this,SLOT(buttonClicked(QString)));      //connecting buttons to signal mapping
            }
            gridLayout->addWidget(&display,newrow,0,1,newcolumn);

            ui->graphicsView->setLayout(gridLayout);                                             //display the DMF device
            mapCreated = true;

            /*---------
             *Reservoirs, Used only if new layout was selected
             * --------*/

            if(newWindowButtonpressed){
                bool ok;                            //variable used for QInputDialog
                                                    //adding reservoirs
                QString text = QInputDialog::getText(this,tr("Next Step"),tr("How many reservoirs do you want?"),QLineEdit::Normal,QDir::home().dirName(),&ok);

                if (text.toInt() == 0){             //if value entered is not a number (text.toInt() will return 0)
                    bool number = false;
                    while (!number)                 //while the user doesn't eneter a number, keep asking
                    {
                       QMessageBox::warning(this,tr("Not a number"), tr("This is not a number, try again"));
                       text = QInputDialog::getText(this,tr("Next Step"),tr("How many reservoirs do you want?"),QLineEdit::Normal,QDir::home().dirName(),&ok);
                       if (text.toInt() == 0)
                       {
                          number = false;
                       }
                       else
                       {
                           number = true;
                       }
                    }
                }
                if(ok&&!text.isEmpty())
                {

                    resnum = text.toInt();          //setting number of reservoirs to be added
                    ui->textEdit->insertPlainText("please select " + text + " reservoirs");
                    addRes = true;                  //need to add the reservoirs
                }
               }

            /*---------
             * Load ContactPadInfo file and fill electrodes with the proper numbering scheme
             * -------*/

            if(!defaultNumbering)
            {
                /*------------------------------------
                 *  Steps for getting contact pad informatoin:
                 * 1.export the contact pad information into a .asc file
                 * 2.Save the file as a text delimited file, a .txt file
                 * The following code does the following
                 * READ TXT FILE CONTAINING ELECTRODE + CONTACT PAD INFORMATION
                 * FILTERS OUT LINES THAT DON'T CONTAIN THE NECESSARY INFO
                 * STORE LINES THAT CONTAIN "T1.", WHICH IS THE RELEVANT INFO
                 * "T1." REPRESENTS RESDES VALUE OF THE ELECTRODE ON DIPTRACE
                 * --------------------------------------*/

                QString fileName;
                if(newWindowButtonpressed)
                {
                    fileName = nLayout->savedNumberingfilename;
                }
                else if(loadWindowButtonpressed)
                {
                    fileName = lLayout->savedContactPadfilename;
                }
                else                                             //Default, need an error handling function
                {
                    fileName = "C:/Users/kaleem/Summer_2016/Steve Shih Project/ContactPadFiles/dmfdesign1.txt";
                }
                QFile file (fileName);
                if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
                    return;
                QTextStream in(&file);
                QString line;
                QStringList a;
                int count =0;
                while (!in.atEnd()) {
                    line = in.readLine();
                    if(line.contains("T1.", Qt::CaseInsensitive)){
                        a.append(line);                         //Store each relevant line in a List
                        count++;
                    }
                }

                /*-----------------------------------------------
                 * THE FOLLOWING CODE SORTS THROUGH THE CONTACT PAD INFO
                 * THE INFO EXPORTED FROM DIPTRACE IS USUALLY RANDOMLY ORDERED
                 * THE INFORMATION IS MANIPULATED TO ENSURE THAT IT IS SORTED IN ASCENDING ORDER AND CAN BE USED TO NUMBER OUR GRID
                 * ------------------------------------------*/

                QStringList b;
                for(int i=0;i<count;i++){
                    b = a.at(i).split(" ");
                    sort(b.begin(),b.end());
                    if(b.at(5).length()<6){
                        int a = 6-b.at(5).length();
                        QStringList c = b.at(5).split(".");
                        QString d;
                        if(a==1){
                            d="0";
                        }
                        else if(a==2){
                            d="00";
                        }
                        else if(a==3){
                            d="000";
                        }
                        b.replace(5,(c.at(0)+"."+d+c.at(1)));                   //CONCATENATES ADDED "0"s
                    }
                    a.replace(i,(b.at(5)+" "+b.at(6)));                         //CONCATENATES CONTACT PADS AND ELECTRODES BACK TOGETHER
                }
                sort(a.begin(),a.end());                                        //SORTS INTO ASCENDING ORDER LIST OF ELECTRODES WITH THEIR RESPECTIVE CONTACT PADS
                QStringList tempCP;
                QRegExp rx("(\\ |\\.)");                                        //RegEx for ' ' or ','
                for(int k = 0;k<count;k++){
                    tempCP = a.at(k).split(rx);
                    ordCP.append(tempCP.at(3));
                }
                for(int w=0;w<ordCP.length();w++){
                        ui->textEdit->insertPlainText(ordCP.at(w)+"\n");        //ONLY DISPLAYS RESPECTIVE CONTACT PADS
                }
                ui->textEdit->insertPlainText("LENGTH: "+ QString::number(ordCP.length()));
                /*-----
                 * NOW THAT THE INFO HAS BEEN SORTED
                 * FILL THE ELECTRODES WITH THEIR RESPECTIVE CONTACT PAD NUMBERS
                 * ----*/
                if(!newWindowButtonpressed){                            //If loadlayout selected, fill the information immediately, otherwise wait until reservoirs are created
                    int z=0;
                    for (int i=0;i<newrow;i++){
                        for (int j=0;j<newcolumn;j++){
                            if(z<ordCP.length() && dmf_array[i][j].text()!=""){
                                dmf_array[i][j].setText(ordCP.at(z));
                                mapper->connect(&dmf_array[i][j],SIGNAL(clicked()),mapper,SLOT(map()));
                                mapper->setMapping(&dmf_array[i][j],QString::number(i)+","+QString::number(j)+","+ordCP.at(z));
                                z++;
                            }
                            else if (z>=ordCP.length()&& dmf_array[i][j].text()!=""){
                                z++;
                            }
                        }
                    }
                    connect(mapper,SIGNAL(mapped(QString)),this,SLOT(buttonClicked(QString)));
                    ui->textEdit->setPlainText("");
                    ui->textEdit->insertPlainText("Number of Contact Pads: " + QString::number(ordCP.length()) + ", Number of Buttons: " + QString::number(z)+"\n");
                    if(z>=ordCP.length()){
                        ui->textEdit->insertPlainText("There are more contact pads than there are buttons");
                       }
                }
            }
    }
    enter_Button_Clicked = true;                 //prevents user from clicking enter twice
    mapCreated = true;                           //Map has been created, may be unnecessary (same function as enter_button_clicked)
    }
    else
    {
        if (enter_Button_Clicked)                //if the user pressed the enter button again, give warning
        {
            QMessageBox::warning(this,tr("Invalid"), tr("You can't click on this button again"));
        }
    }
}

/*
 * Function determining what happens when a button is clicked
 * Action taken when button is clicked depends on the state of the system
 */

void DMFgui::buttonClicked(QString text)
{
    QStringList electrodeList;
    electrodeList = text.split(",");                    //y_coordinate,x_coordinate,electrode_number <- electrodeList
    if(!deleteButtonchoice){
        ClearColor();                                   //when a button is clicked, reset the color of all buttons
        if (elec ==1){                                  //saving values into the first electrode (struct)
            electrode_1.x = electrodeList.value(1).toInt();
            electrode_1.y = electrodeList.value(0).toInt();
            elec ++;                                    //go to electrode_2 next time a button is pressed
        }
        else if(elec==2)
        {
            electrode_2.x = electrodeList.value(1).toInt();
            electrode_2.y = electrodeList.value(0).toInt();
            elec --;                                    //go to electrode_1 next time a button is pressed
        }
        if (addRes){                                    //if the signal for adding a reservoir was called, add a reservoir
            if (added<resnum)
            {
               if (add_reservoir(newcolumn,newrow,resnum))
               {
                   added++;
               }
            }
            else
            {
                /*-----------
                 *Fill out buttons after creating reservoirs
                 * May need to look for a better solution
                 * If you have a preloaded contact pads information
                 * ----------*/

                if(!defaultNumbering){
                    disconnect(mapper,SIGNAL(mapped(QString)),this,SLOT(buttonClicked(QString)));

                    ui->textEdit->setPlainText("x: "+electrodeList.value(1) +  ",y: " +electrodeList.value(0));
                    int z=0;
                    for (int i=0;i<newrow;i++)
                    {
                        for (int j=0;j<newcolumn;j++)
                            {
                            if(z<ordCP.length() && dmf_array[i][j].text()!=""){
                                dmf_array[i][j].setText(ordCP.at(z));
                                mapper->disconnect(&dmf_array[i][j],SIGNAL(clicked()),mapper,SLOT(map()));                          //Disconnect previous connection before establishing new connection
                                mapper->connect(&dmf_array[i][j],SIGNAL(clicked()),mapper,SLOT(map()));
                                mapper->setMapping(&dmf_array[i][j],QString::number(i)+","+QString::number(j)+","+ordCP.at(z));     // numberingcount-1));//QString::number(numberingcount));
                                z++;
                            }
                            else if (z>=ordCP.length()&& dmf_array[i][j].text()!=""){
                                z++;
                            }
                        }
                    }


                    connect(mapper,SIGNAL(mapped(QString)),this,SLOT(buttonClicked(QString)));
                    ui->textEdit->setPlainText("");
                    ui->textEdit->insertPlainText("Number of Contact Pads: " + QString::number(ordCP.length()) + ", Number of Buttons: " + QString::number(z)+"\n");
                    if(z>=ordCP.length()){
                        ui->textEdit->insertPlainText("There are more contact pads than there are buttons");
                   }
                }

                addRes = false;                  //signal for adding reservoirs finished
                QMessageBox::warning(this,tr("Next Step"), tr("You can start generating paths"));
            }
        }

        else
        {
            if(realTime){                               //If real-time sequencing is selected, turn on the electrode immediately
                if (arduino->isWritable()){             //Communicate with arduino
                    QString a = "1000," + electrodeList.value(2);
                    const char * dat = a.toStdString().c_str();
                    arduino->write(dat);
                    ui->textEdit->insertPlainText("\nElectrode: "+electrodeList.value(2)+" was sent.\n");
                }
                else{
                    QMessageBox::warning(this,tr("Real-time sequencing"), tr("Communication with the Arduino was unsuccessful\nPlease Try Again"));
                }
            }
            else{
                save_to_String(electrodeList.value(2));
            }
        }
    }
    /*---------
     * Delete Buttons
     * -------*/
    else{
        electrode_1.x = electrodeList.value(1).toInt();
        electrode_1.y = electrodeList.value(0).toInt();
        dmf_array[electrode_1.y][electrode_1.x].setText("");            //Set text to empty
        dmf_array[electrode_1.y][electrode_1.x].setStyleSheet("background-color: white;border-color:none;border-style: outset ;");
        dmf_array[electrode_1.y][electrode_1.x].setEnabled(false);      //Ensure that it is no longer clickable
        ui->textEdit->setPlainText("Electrode: " + electrodeList.value(2) + " was deleted");
    }
}
/*
 * Function for adding a reservoir
 */
bool DMFgui::add_reservoir(int column, int row, int resnum)
{
    int * coords = getRecent_Coordinates();                           //getting most recent coordinates and saving them in a new array
    int x_coord = coords[2];
    int y_coord = coords[3];

// COMMENTED OUT: MIGHT NOT NEED IT
//    QPushButton *reservoir = new QPushButton;                    //adding new Reservoir button  #can it be clicked?
//    reservoir->setText("res");
//    reservoir->setEnabled(false);                                //cannot press this button
//    reservoir->setStyleSheet( "border-style: outset ;border-width: 2px; border-color: grey");

    QStringList emptySpaceList = findEmptySpace(y_coord,x_coord).split(",");
    int caseSwitch;

    if (emptySpaceList.contains("0",Qt::CaseSensitive)){
        caseSwitch = 0;
    }
    if (emptySpaceList.contains("1",Qt::CaseSensitive)){
        caseSwitch = 1;
    }
    if (emptySpaceList.contains("2",Qt::CaseSensitive)||emptySpaceList.contains("3",Qt::CaseSensitive)){
        caseSwitch = 2;
    }

    switch (caseSwitch){
        case 2:
            numberingcount++;

            dialog = new Dialog();
            dialog->choice(emptySpaceList);
            dialog->exec();
            location = dialog->saved;

            if (location=="top"){
                setMapping(y_coord,x_coord-1);
                map_reservoir(y_coord,x_coord-2);
                return true;
                break;
            }
            if (location=="left"){
                setMapping(y_coord-1,x_coord);
                map_reservoir(y_coord-2,x_coord);
                return true;
                break;
            }
            if (location=="right"){
                setMapping(y_coord+1,x_coord);
                map_reservoir(y_coord+2,x_coord);
                return true;
                break;
            }
            if (location=="bottom"){
                setMapping(y_coord,x_coord+1);
                map_reservoir(y_coord,x_coord+2);
                return true;
                break;
            }
            return true;
            break;
        case 1:
            //add new buttons depending on where the button clicked is (reservoir is hatched)
                numberingcount++;

                //set booleans so that you don't get to click to same place twice
                if (emptySpaceList.contains("left",Qt::CaseSensitive))//left column
                {
                    setMapping(y_coord-1,x_coord);
                    map_reservoir(y_coord-2,x_coord);
                    return true;
                    break;
                }
                else if (emptySpaceList.contains("right",Qt::CaseSensitive)) //right column
                {
                    setMapping(y_coord+1,x_coord);
                    map_reservoir(y_coord+2,x_coord);
                    return true;
                    break;
                }
                else if (emptySpaceList.contains("top",Qt::CaseSensitive))
                {
                    setMapping(y_coord,x_coord-1);
                    map_reservoir(y_coord,x_coord-2);
                    return true;
                    break;
                }
                else if(emptySpaceList.contains("bottom",Qt::CaseSensitive))
                {
                    setMapping(y_coord,x_coord+1);
                    map_reservoir(y_coord,x_coord+2);
                    return true;
                    break;
                }
        case 0:
            QMessageBox::warning(this,tr("Invalid"), tr("You can't put a reservoir here"));
            return false;
            break;
    }
}
/*
 * Getting most recent coordinate (either electrode_1 or electrode_2)
 */
int * DMFgui::getRecent_Coordinates(){
    int recentCoord[4];
    if (elec==1)
    {
        recentCoord[0] = electrode_1.y;
        recentCoord[2] = electrode_2.y;
        recentCoord[1] = electrode_1.x;
        recentCoord[3] = electrode_2.x;
    }
    else if (elec==2)
    {
        recentCoord[0] = electrode_2.y;
        recentCoord[2] = electrode_1.y;
        recentCoord[1] = electrode_2.x;
        recentCoord[3] = electrode_1.x;
    }
    return recentCoord;
}

/*
 * Setting mapping for all the buttons created
 */
void DMFgui::setMapping(int x, int y)
{
    dmf_array[y][x].setText(QString::number(numberingcount));
    dmf_array[y][x].setStyleSheet( "border-style: outset ;border-width: 2px; border-color: grey");
    gridLayout->addWidget(&dmf_array[y][x],y,x);
    mapper->connect(&dmf_array[y][x],SIGNAL(clicked()),mapper,SLOT(map()));
    mapper->setMapping(&dmf_array[y][x],QString::number(y)+","+QString::number(x)+","+QString::number(numberingcount));
}
/*
 * Adding a reservoir and setting it on the map
 * So that it becomes clickable
 * TODO: DISABLE RESERVOIR BEING CLICKED WHEN ADDING RESERVOIRS!!!
 */
void DMFgui::map_reservoir(int x, int y){
    numberingcount++;
    reservoircount++;
    //dmf_array[y][x].setText("res no:" + QString::number(reservoircount));
    dmf_array[y][x].setText(QString::number(numberingcount));
    gridLayout->addWidget(&dmf_array[y][x],y,x);
    dmf_array[y][x].setStyleSheet( "border-style: outset ;border-width: 2px; border-color: grey");
    mapper->connect(&dmf_array[y][x],SIGNAL(clicked()),mapper,SLOT(map()));
    mapper->setMapping(&dmf_array[y][x],QString::number(y)+","+QString::number(x)+","+QString::number(numberingcount));
}
/*--------------------------------------------------------------------------------------------------------
 *
 * Path Generation related Functions
 * so far: uses arrays
 * # PRIORITY: consider using LinkedList for autopath generation
 *
--------------------------------------------------------------------------------------------------------*/
/*
 * saves most recent coordinates to the autogen path (saves beginning and end)
 */
void DMFgui::on_autogen_Button_clicked(){
    int *z = getRecent_Coordinates();

    int a1 = z[0];
    int a2 = z[1];
    int a3 = z[2];
    int a4 = z[3];

    QString route = autoGeneratePath(a2,a1,a4,a3);
}

/*
 * saves most recent coordinates to the autogen path (saves beginning and end)
 */
QString DMFgui::autoGeneratePath(const int & xStart,const int & yStart,const int & xFinish,const int & yFinish){
    //node sets

    int **closedNodes = new int*[horizontalSize];
    int **openNodes = new int*[horizontalSize];
    int **dir_map = new int*[horizontalSize];
    for (int i=0;i<verticalSize;i++){
        closedNodes[i] = new int[verticalSize];                 //set of nodes that are already evaluated and marked off
        openNodes[i] = new int[verticalSize];                   //set of nodes to be evaluated
        dir_map[i] = new int[verticalSize];                     //map of directions(parent-child connection) keeps track of the different paths
    }

    //directions
    const int dir = 4;
    static int dirX[dir]={1,0,-1,0}; //x direction
    static int dirY[dir]={0,1,0,-1}; //y direction

    static int index;      //initialized to 0?                                       //static and global variables are initialized to 0
    QStack<node> stack[2];
    static node *currentNode;
    static node *neighborNode;

  //reset map
    for (int y=0;y<horizontalSize;y++){
        for (int x=0;x<verticalSize;x++){
            closedNodes[x][y]=0;
            openNodes[x][y]=0;
        }
    }

    //create the start node
    static node *startNode;
    startNode = new node(xStart,
                         yStart,
                         0,
                         0);
    startNode->updatePriority(xFinish,yFinish);
    activate(startNode->getyPos(),startNode->getxPos(),0);                                       // MIGHT NOT NEED IT sets the start node as blue

    //push the start node to the priority queue
    stack[index].push(*startNode);
    //add to the list of open nodes in order to evaluate the start node
    openNodes[startNode->getxPos()][startNode->getyPos()] = startNode->getPriority();

    ui->textEdit->insertPlainText("\n before loop\n");

    //while priority queue is not empty, continue
    while(!stack[index].empty()){

        ui->textEdit->insertPlainText("\n start of loop\n");
        auto min = std::min_element( stack[index].begin(), stack[index].end(),
                                     []( const node &a, const node &b )
                                     {
                                         return a.getPriority() < b.getPriority();
                                     } );

        //get the current node with the highest priority in the priority list (this will be the smallest value, according to the way we set this up)
        currentNode = new node(min->getxPos(),
                               min->getyPos(),
                               min->getDistance(),
                               min->getPriority());
        //remove the current node from the priority queue
        stack[index].erase(min);
        //add the currentNode into the openNodes (this is actually building a parallel map that stores what to evaluate)
        openNodes[currentNode->getxPos()][currentNode->getyPos()]=0;
        //add the currentNode into the closedNodes
        closedNodes[currentNode->getxPos()][currentNode->getyPos()]=1;

        ui->textEdit->insertPlainText("\n\n XPos:"+ QString::number(currentNode->getxPos()));
        ui->textEdit->insertPlainText("YPos:"+ QString::number(currentNode->getyPos()) + "\n");

        //check if target has been reached
        if (currentNode->getxPos()==xFinish && currentNode->getyPos()==yFinish){
            ui->textEdit->insertPlainText("target has been reached");                            //activates the destination node
            //return the generated path in a QString
            QString path = "";
            int x = currentNode->getxPos();
            int y = currentNode->getyPos();
            activate(y,x,2);
            //back track the whole procress to build a QString
            while (!(x==xStart && y ==yStart))
            {
            ui->textEdit->insertPlainText("\nxStart: "+ QString::number(xStart));
            ui->textEdit->insertPlainText("\nyStart: "+ QString::number(yStart));
            ui->textEdit->insertPlainText("\nxBefore: "+ QString::number(x));
            ui->textEdit->insertPlainText("\nyBefore: "+ QString::number(y));
                int direction = dir_map[x][y];
  //                path = (direction + dir/2)%dir + path; //CHECK THIS OUT
                path = dmf_array[y][x].text()+","+path;
                ui->textEdit->insertPlainText("\ndirection: " + QString::number(direction));
                x += dirX[direction]; //direction is pointing towards the previous node
                y += dirY[direction];
                ui->textEdit->insertPlainText("\n\nxAfter: "+ QString::number(x));
                ui->textEdit->insertPlainText("\nyAfter: "+ QString::number(y));
                activate(y,x,1);
            }
            //for the start node
            activate(y,x,0);
            path = dmf_array[y][x].text() + path;
            //delete the node
            delete currentNode;
            //emptying the 'new' node
            while (!stack[index].empty()){
                stack[index].pop();
            }
            ui->textEdit->insertPlainText("\n\n path: "+ path);
            return path;
        break;
        }
        //if target hasn't been reached, add all the neighboring nodes to the OpenList + define parents for later tracing
        else{
            buildArray(currentNode->getyPos(),currentNode->getxPos());
            for (int j=0;j<avail;j++){
                int i = eavail[j];
                /** coordinates are correct **/
                int neighborX = currentNode->getxPos()+dirX[i];
                int neighborY = currentNode->getyPos()+dirY[i];

                ui->textEdit->insertPlainText("\n neighborX:"+ QString::number(neighborX));
                ui->textEdit->insertPlainText("\n neighborY:"+ QString::number(neighborY));
                //means that it's unavailable

            if (dmf_array[neighborY][neighborX].text()==""){
                closedNodes[neighborX][neighborY]=1;
                ui->textEdit->insertPlainText(" in closedNodes");
            }

            //TODO: use the code writen for check if avaiable or something
            //ignore if it's on the closed list or if it has been marked as unavailable

            else if (!(neighborX<0||neighborY<0||neighborX>horizontalSize||neighborY>verticalSize||closedNodes[neighborX][neighborY]==1)){
                //if it gets here, then generate a neighbor Node
                neighborNode = new node(neighborX,
                                        neighborY,
                                        currentNode->getDistance(),
                                        currentNode->getPriority());
                /** coordinates are ok, this works fine **/
                ui->textEdit->insertPlainText("\n old priority:"+ QString::number(neighborNode->getPriority()));
                ui->textEdit->insertPlainText("\n old distance:"+ QString::number(neighborNode->getDistance()));
                /** this seems to be ok **/
                neighborNode->updateDistance();
                neighborNode->updatePriority(xFinish,yFinish);
                ui->textEdit->insertPlainText("\n new priority:"+ QString::number(neighborNode->getPriority()));
                ui->textEdit->insertPlainText("\n new distance:"+ QString::number(neighborNode->getDistance()));

                //if neighborNode is not in the openNodes, add it
                if (openNodes[neighborX][neighborY]==0){
                    ui->textEdit->insertPlainText("\n adding to openNodes");
                    //add it
                    openNodes[neighborX][neighborY] =neighborNode->getPriority();
                    //add it to the priority cue
                    stack[index].push(*neighborNode);
                    //set the parent
                    dir_map[neighborX][neighborY]=(i+dir/2)%dir; //look at notebook for references
                    ui->textEdit->insertPlainText("\n added to openNodes");
                }
                //if neighborNode is already on the openNodes, check if the path to that one is better
                else if (openNodes[neighborX][neighborY]>neighborNode->getPriority()){
                    ui->textEdit->insertPlainText("\n already on openNodes");
                    //updating fscore inside openNodes
                    openNodes[neighborX][neighborY] = neighborNode->getPriority();
                    //update the parent node
                    dir_map[neighborX][neighborY]=(i+dir/2)%dir;
                    while (!(min->getxPos()==neighborX && min->getyPos()==neighborY)){
                        stack[1-index].push(*min);
                        stack[index].erase(min);
                    }
                    stack[index].erase(min); //remove the -old-current node

                    ui->textEdit->insertPlainText("\n stage 1");

                    /** ?? **/
                    if(stack[index].size()>stack[1-index].size()){ //??? is this extra check necessary?
                        index=1-index; //index switch 1->0 or 0->1
                        ui->textEdit->insertPlainText("\n switching index");
                    }

                    //while(!stack[index].empty()){
                        stack[1-index].push(*min);
                        ui->textEdit->insertPlainText("\n stack[1-index].push(*min);");
                        stack[index].erase(min); // fix this??
                        ui->textEdit->insertPlainText("\n transfering finished");
                    //}
                    /** ?? **/
                    index=1-index; //index switch 1->0 or 0->1
                    stack[index].push(*neighborNode); //and the -new-current node will be pushed in instead
                    ui->textEdit->insertPlainText("\n finished already on openNodes");
                }
                else delete neighborNode;
            }
  //            delete[]eavail; //deletes the array so you can recreate a new one next time
        }
            delete currentNode;
    } autoGen = true;
        ui->textEdit->insertPlainText("\n end of loop\n");
}
    return ""; //no path found
}
/*
 * Analyzing top, bottom, left and right to the target electrode
 * returns which electrodes are available by determining if they have something
 * written on them or not
 *
 */
void DMFgui::buildArray(int y, int x){
    if (y ==0 && x==0){ //top left corner
        avail =2;
        eavail = new int[avail];
        eavail[0]=0;    //right
        eavail[1]=1;    //bottom
    }
    else if(y==0&&x==(newcolumn-1)){ //top right corner
        avail =2;
        eavail = new int[avail];
        eavail[0]=2;    //left
        eavail[1]=1;    //bottom
    }
    else if(y==(newrow-1)&&x==0){ //bottom left corner
        avail =2;
        eavail = new int[avail];
        eavail[0]=0;    //right
        eavail[1]=3;    //top
    }
    else if(y==(newrow-1)&&x==(newcolumn-1)){ //bottom right corner
        avail =2;
        eavail = new int[avail];
        eavail[0]=2;    //left
        eavail[1]=3;    //top
    }
    else if(y==0){ //top row
        avail =3;
        eavail = new int[avail];
        eavail[0]=0;    //right
        eavail[1]=2;    //left
        eavail[2]=1;    //bottom
    }
    else if(y==(newrow-1)){ //bottom row
        avail =3;
        eavail = new int[avail];
        eavail[0]=2;    //left
        eavail[1]=3;    //top
        eavail[2]=0;    //right
    }
    else if(x==0){ //left column
        avail =3;
        eavail = new int[avail];
        eavail[0]=3;    //top
        eavail[1]=1;    //bottom
        eavail[2]=0;    //right
    }
    else if(x==(newcolumn-1)){ //right column
        avail =3;
        eavail = new int[avail];
        eavail[0]=2;    //left
        eavail[1]=1;    //bottom
        eavail[2]=3;    //top
    }
    else{
        avail =4;
        eavail = new int[avail];
        eavail[0]=2;    //left
        eavail[1]=1;    //bottom
        eavail[2]=3;    //top
        eavail[3]=0;    //right
    }
}

//finds which electrodes are available next to the current electrode
QString DMFgui::findAvailableSpace(int y, int x){
    QString availElec = "";

    //check on top of the current electrode
    if (dmf_array[y-1][x].text()!="")
    {
        availElec += "top,";
    }
    //check bottom of current electrode
    if (dmf_array[y+1][x].text()!="")
    {
        availElec += "bottom,";
    }
    //check to the left of current electrode
    if (dmf_array[y][x-1].text()!="")
    {
        availElec += "left,";
    }
    //check to the right of current electrode
    if (dmf_array[y][x+1].text()!="")
    {
        availElec += "right,";
    }
    return availElec;
}

/*
 * Does the opposite of findAvailableSpace
 */
QString DMFgui::findEmptySpace(int x, int y){
    int emptySpaces = 0;                                         //keeps track of how many empty spaces there are
    QString emptyElec = "";
    if (dmf_array[y-1][x].text()==""){                           //check on top of the current electrode
        emptyElec += "top,";
        emptySpaces ++;
    }
    if (dmf_array[y+1][x].text()==""){                           //check bottom of current electrode
        emptyElec += "bottom,";
        emptySpaces++;
    }
    if (dmf_array[y][x-1].text()==""){                           //check to the left of current electrode
        emptyElec += "left,";
        emptySpaces++;
    }
    if (dmf_array[y][x+1].text()==""){                           //check to the right of current electrode
        emptyElec += "right,";
        emptySpaces++;
    }
    emptyElec += QString::number(emptySpaces);
    ui->textEdit->insertPlainText("\nempty Electrodes: " + emptyElec);
    ui->textEdit->insertPlainText("\nnumber of empty spaces: " + QString::number(emptySpaces));
    return emptyElec;
}

/*
 * save_to_String whichever electrode was selected for the autogenerated path
 * sets the electrode green
 */
void DMFgui::activate(int y, int x,int position){
    if (position == 0){                                          //start
        dmf_array[y][x].setStyleSheet("background-color:blue; border-style: outset ;border-width: 2px; border-color: grey");
    }
    if(position==1){
        dmf_array[y][x].setStyleSheet("background-color:green; border-style: outset ;border-width: 2px; border-color: grey");
    }
    if(position==2){
        dmf_array[y][x].setStyleSheet("background-color:red; border-style: outset ;border-width: 2px; border-color: grey");
    }
//    save_to_String(dmf_array[y][x].text());
//    dmf_array[y][x].setStyleSheet("background-color:green; border-style: outset ;border-width: 2px; border-color: grey");
}


/*
 * clears the color of all the electrodes on the layout
 * based on arrays for now
 * #think about using LinkedList
 */
void DMFgui::ClearColor(){
    if(autoGen){
        for (int i=0;i<newrow;i++){         //naming and setting properties for each button
            for (int j=0;j<newcolumn;j++){
                if(dmf_array[i][j].text() != ""){
                    dmf_array[i][j].setStyleSheet("border-style:outset;border-width:2px;border-color:grey");
                }
            }
        }

    }
    autoGen = false;
}

/*--------------------------
 * Allows user to turn on electrodes instantly after pressing the button
 * It only allows for sequencing
 * --------------------------*/
void DMFgui::on_realTimeBox_clicked(bool checked)
{
    if (checked){
        realTime = true;
    }
    else
        realTime = false;
}

/*---------
 * If delete button is pressed, you can  buttons
 * It will be used to create layouts that can also be saved later
 * ------*/

void DMFgui::on_DeleteBox_clicked(bool checked)
{
    if (checked){
        QMessageBox::warning(this,tr("Caution"), tr("Once a button is deleted, it cannot be undone"));
        deleteButtonchoice=true;
    }
    else {
        deleteButtonchoice = false;
    }
}

/*--------------------------------------------------------------------------------------------------------
 *
 * Function Generator related Functions
 * #TODO: open and close function generator
 *        control the Frequency in addition to the voltage
 *
--------------------------------------------------------------------------------------------------------*/
/*
 * CLASS CALL: funcGen()
 * Sends a voltage for the function generator
 */

void DMFgui::on_Voltage_SendButton_clicked(){
    QString voltage = ui->lineEdit->text(); //gets the text that you've enteredo
    float to_Send = voltage.toFloat();

    funcgen = new funcGen();
    funcgen->send_voltage(to_Send);
}

/*--------------------------------------------------------------------------------------------------------
 *
 * nemeSYS related Functions
 * #TO DO: find a better way to check if nemeSYS is open or not
 *
--------------------------------------------------------------------------------------------------------*/
/*
 * setting target Volume
 * disables setting FlowRate
 */

void DMFgui::on_targetVolume_clicked(){
    ui->targetFlowRateEdit->setReadOnly(true);
    ui->targetVolumeEdit->setReadOnly(false);                    //allowing for the value to be changed
    volume = ui->targetVolumeEdit->text().toDouble();
}

/*
 * setting target FlowRate
 * disables setting Volume
 */
void DMFgui::on_targetFlow_clicked(){
    ui->targetVolumeEdit->setReadOnly(true);
    ui->targetFlowRateEdit->setReadOnly(false);                  //allowing for the value to be changed
    flowRate = ui->targetFlowRateEdit->text().toDouble();
}

/*
 * display error message if nemeSYS not open
 */
void DMFgui::nemesysNotOpenedErrorMessage(){
    QMessageBox::warning(this,tr("NemeSYS not Opened"), tr("Please open NemeSYS first"));
}
/*
 * display error message if nemeSYS already open
 */
void DMFgui::nemesysAlreadyOpenedMessage(){
    QMessageBox::warning(this,tr("NemeSYS Opened"), tr("NemeSYS is already opened"));
}
/*
 * display error message if nemeSYS is busy
 */
void DMFgui::nemesysDosingMessage(){
    QMessageBox::warning(this,tr("NemeSYS is Dosing"), tr("Please wait until Dosing Unit is finished Dosing/Calibrating"));
}

/*
 * display error message if nemeSYS is calibrating
 * #May be unnecessary, to be confirmed
 */
void DMFgui::nemesysCalibrateMessage(){
    QMessageBox::warning(this,tr("NemeSYS is Calibrating"), tr("Please wait until Dosing Unit is finished Calibrating"));
}
/*
 * opening nemeSYS
 */
void DMFgui::on_OpenButton_clicked(){
    nemesys = new Nemesys();

    if (!opened){
        nemesys->openConnection();
    }
    else{
        nemesysAlreadyOpenedMessage();
    }
    opened = true;
}
/*
 * closing nemeSYS
 */
void DMFgui::on_CloseButton_clicked(){
    if (opened){
        nemesys->closeConnection();
        opened = false;
    }
    else{
        nemesysNotOpenedErrorMessage();
    }
}


/*
 * dosing from syringe
 * For now, put a positive volume and negative flow
 */
void DMFgui::on_doseButton_clicked(){
    if (opened && nemesys->CheckDosingStatus((unsigned char)(ui->dosingUnitscomboBox->currentIndex()))== 1)
    {
        unsigned char dosingUnit = (unsigned char)(ui->dosingUnitscomboBox->currentIndex());        //which syringe to use

        double volume = ui->targetVolumeEdit->text().toDouble();
        double flow = ui->targetFlowRateEdit->text().toDouble();

        unsigned char volumeUnit = (unsigned char)(ui->unitsComboBox->currentIndex());
        nemesys->setActiveVolumeUnit(dosingUnit, volumeUnit);
        unsigned char flowUnit = (unsigned char)(ui->funitscomboBox->currentIndex());
        nemesys->setActiveFlowUnit(dosingUnit, flowUnit);

        nemesys->DoseVolume(dosingUnit, volume,flow);
    }
    else if (opened && nemesys->CheckDosingStatus((unsigned char)(ui->dosingUnitscomboBox->currentIndex()))== 0){
        nemesysDosingMessage();
    }

    //May be unnecessary, to be confirmed
    else if (opened && nemesys->CheckCalibrateStatus((unsigned char)(ui->dosingUnitscomboBox->currentIndex()))== 0){
        nemesysCalibrateMessage();
    }
    else if (!opened){
        nemesysNotOpenedErrorMessage();
    }
}
/*
 * emptying syringe
 * For now put a positive flow value
 */
void DMFgui::on_emptyButton_clicked()
{
    //Open and not dosing or calibrating
    if (opened && nemesys->CheckDosingStatus((unsigned char)(ui->dosingUnitscomboBox->currentIndex()))== 1)
    {
        unsigned char dosingUnit = (unsigned char)(ui->dosingUnitscomboBox->currentIndex());

        double flow = ui->targetFlowRateEdit->text().toDouble();
        unsigned char flowUnit = (unsigned char)(ui->funitscomboBox->currentIndex());
        nemesys->setActiveFlowUnit(dosingUnit, flowUnit);

        nemesys->EmptySyringe(dosingUnit, flow);                                                //only flow matters for emptying
    }
    else if (opened && nemesys->CheckDosingStatus((unsigned char)(ui->dosingUnitscomboBox->currentIndex()))== 0) {
         nemesysDosingMessage();
    }
    //May be unnecessary, to be confirmed
    else if (opened && nemesys->CheckCalibrateStatus((unsigned char)(ui->dosingUnitscomboBox->currentIndex()))== 0){
        nemesysCalibrateMessage();
    }
    else if (!opened){
         nemesysNotOpenedErrorMessage();
    }
}
/*
 * refilling syringe
 * For now, ensure that you have a negative flow value
 */
void DMFgui::on_refillButton_clicked()
{
    //Open and not dosing or calibrating
    if (opened && nemesys->CheckDosingStatus((unsigned char)(ui->dosingUnitscomboBox->currentIndex()))== 1)
    {
        unsigned char dosingUnit = (unsigned char)(ui->dosingUnitscomboBox->currentIndex());

        double flow = ui->targetFlowRateEdit->text().toDouble();
        unsigned char flowUnit = (unsigned char)(ui->funitscomboBox->currentIndex());
        nemesys->setActiveFlowUnit(dosingUnit, flowUnit);

        nemesys->RefillSyringe(dosingUnit, flow);
    }
    else if (opened && nemesys->CheckDosingStatus((unsigned char)(ui->dosingUnitscomboBox->currentIndex()))== 0){
         nemesysDosingMessage();
    }
    //May be unnecessary, to be confirmed
    else if (opened && nemesys->CheckCalibrateStatus((unsigned char)(ui->dosingUnitscomboBox->currentIndex()))== 0){
        nemesysCalibrateMessage();
    }
    else if (!opened){
         nemesysNotOpenedErrorMessage();
    }
}
/*
 * Calibrates nemeSYS
 */
void DMFgui::on_CalibrateButton_clicked()
{
    //Open and not dosing
    if (opened && nemesys->CheckDosingStatus((unsigned char)(ui->dosingUnitscomboBox->currentIndex()))== 1)// && nemesys->CheckCalibrateStatus((unsigned char)(ui->dosingUnitscomboBox->currentIndex()) == 1))
    {

        //Warning Message to make sure no syringes are placed
        QMessageBox calibrateBox;
        calibrateBox.setWindowTitle("Calibration Warning");
        calibrateBox.setText("Please ensure that no syringes are placed in the dosing unit.\nExecuting the calibration move with a syringe fitted on the device may cause damage to the syringe.\n\nAre you ready?");
        QAbstractButton* pButtonYes = calibrateBox.addButton(tr("Yes"), QMessageBox::YesRole);
        calibrateBox.addButton(tr("No"), QMessageBox::NoRole);

        calibrateBox.exec();

        //Execute calibration if user is ready
        if (calibrateBox.clickedButton()==pButtonYes) {
            unsigned char dosingUnit = (unsigned char)(ui->dosingUnitscomboBox->currentIndex());

            unsigned char volumeUnit = (unsigned char)(ui->unitsComboBox->currentIndex());
            nemesys->setActiveVolumeUnit(dosingUnit, volumeUnit);

            unsigned char flowUnit = (unsigned char)(ui->funitscomboBox->currentIndex());
            nemesys->setActiveFlowUnit(dosingUnit, flowUnit);

            nemesys->CalibrateUnit(dosingUnit);
            nemesys->CheckCalibrateStatus(dosingUnit);
        }
    }
    else if (opened && nemesys->CheckDosingStatus((unsigned char)(ui->dosingUnitscomboBox->currentIndex()))== 0){
        nemesysDosingMessage();

    }
    else if (opened && nemesys->CheckCalibrateStatus((unsigned char)(ui->dosingUnitscomboBox->currentIndex()))== 0){
        nemesysCalibrateMessage();
    }
    else if (!opened){
        nemesysNotOpenedErrorMessage();
    }

}
/*
 * Stopping nemeSYS
 */
void DMFgui::on_StopButton_clicked()
{
    if(!opened){
        nemesysNotOpenedErrorMessage();
    }
    else{
        unsigned char dosingUnit = (unsigned char)(ui->dosingUnitscomboBox->currentIndex());
        nemesys->StopUnit(dosingUnit);
    }
}

/*
 * Monitoring functions for the nemesys
 * Updates the syringe level and flow rate values when the Monitor button is clicked
 * When the Monitor box is checked it sets into action the following function:
 * The monitor box will repeatedly be clicked over a certain time interval
 * Each dosing unit has it's own function
 */


void DMFgui::on_MonitorBox1_clicked(bool checked)
{
    if (checked && opened)
    {
        int rate = ui->targetRefreshRateEdit->text().toInt();
        ui->MButton1->setDown(true);
        ui->MButton1->setAutoRepeat(true);                  //Set MButton into a repetitive clicking loop
        ui->MButton1->setAutoRepeatDelay(rate);             //Set the time delay
        ui->MButton1->setAutoRepeatInterval(rate);          //Set the time interval
    }
    else if (!opened)
    {
        nemesysNotOpenedErrorMessage();                     //Display an error if the nemesys isn't opened
        ui->MonitorBox1->setChecked(false);
    }
    else
    {
        ui->MButton1->setAutoRepeat(false);                 //Remove MButton from the repetitive clicking
        ui->MButton1->setDown(false);
    }
}

void DMFgui::on_MonitorBox2_clicked(bool checked)
{
    if (checked && opened)
    {
        int rate = ui->targetRefreshRateEdit->text().toInt();
        ui->MButton2->setDown(true);
        ui->MButton2->setAutoRepeat(true);
        ui->MButton2->setAutoRepeatDelay(rate);
        ui->MButton2->setAutoRepeatInterval(rate);
    }
    else if (!opened)
    {
        nemesysNotOpenedErrorMessage();
        ui->MonitorBox2->setChecked(false);
    }
    else
    {
        ui->MButton2->setAutoRepeat(false);
        ui->MButton2->setDown(false);
    }
}

void DMFgui::on_MonitorBox3_clicked(bool checked)
{
    if (checked && opened)
    {
        int rate = ui->targetRefreshRateEdit->text().toInt();
        ui->MButton3->setDown(true);
        ui->MButton3->setAutoRepeat(true);
        ui->MButton3->setAutoRepeatDelay(rate);
        ui->MButton3->setAutoRepeatInterval(rate);
    }
    else if (!opened)
    {
        nemesysNotOpenedErrorMessage();
        ui->MonitorBox3->setChecked(false);
    }
    else
    {
        ui->MButton3->setAutoRepeat(false);
        ui->MButton3->setDown(false);
    }
}

void DMFgui::on_MButton1_clicked()
{
    double actualFlow = nemesys->getActualFlowRate(0) ;
    double actualSyringe = nemesys->getActualSyringeLevel(0);

    ui->TargetMonitorFlowRate_1->setText(QString::number(actualFlow));
    ui->TargetMonitorSyringeLevel_1->setText(QString::number(actualSyringe));
}

void DMFgui::on_MButton2_clicked()
{
    double actualFlow = nemesys->getActualFlowRate(1) ;
    double actualSyringe = nemesys->getActualSyringeLevel(1);

    ui->TargetMonitorFlowRate_2->setText(QString::number(actualFlow));
    ui->TargetMonitorSyringeLevel_2->setText(QString::number(actualSyringe));
}

void DMFgui::on_MButton3_clicked()
{
    double actualFlow = nemesys->getActualFlowRate(2) ;
    double actualSyringe = nemesys->getActualSyringeLevel(2);

    ui->TargetMonitorFlowRate_3->setText(QString::number(actualFlow));
    ui->TargetMonitorSyringeLevel_3->setText(QString::number(actualSyringe));
}

/*
 * Monitoring mouse movement
 * #is this even useful?
 */
void DMFgui::mousePressEvent(QMouseEvent *e)
{
//    if (e->button() == Qt::LeftButton)
//    {
//        ui->textEdit->insertPlainText("Pressed"); //double check this, it's appearing with the line edit
//    }
}

/*---------------------------------------
 *
 *
 * Functions for Writing/Reading Sequence files
 * Preview electrodes to be turned on
 * Allow users to write to Montior
 *
 *
 * ---------------------------------------*/

/*------------
 * Write the sequence that is in the serial monitor to a text file
 * ----------*/
void DMFgui::on_writeButton_clicked()
{
    if (!mapCreated){
        mapNotCreatedErrorMessage();
    }
    else{
        //The directory in which the file is saved to is set here
        QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save Text files"), "C:/Users/kaleem/Summer_2016/Steve Shih Project/SequenceFiles", tr("Text Files (*.txt)"));

        QFile file (fileName);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            return;
        QTextStream out(&file);
        out << ui->textEdit->toPlainText(); //All the information that is in the serial monitor is written to the saved file
                                            //May be better to set the information that is to be saved to the QString to_Send
    }
}

/*
 * Read data from a text file and display on to serial monitor
 * "line" contains the sequence
 * ----------*/

void DMFgui::on_ReadButton_clicked()
{
    if (!mapCreated){
        mapNotCreatedErrorMessage();
    }
    else{

        //The directory in which the file is opened from is set here
        QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Text files"), "C:/Users/kaleem/Summer_2016/Steve Shih Project/SequenceFiles", tr("Text Files (*.txt)"));

        QFile file (fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return;
        QTextStream in(&file);
        QString line;

        //QString is suppose to append the next line after the previous one, however this isn't working exactly as expected, instead the line is inserted before
        while (!in.atEnd()) {
             line = in.readLine().insert(0,line);
        }
        ui->textEdit->setPlainText(line);
    }
 }


/*------------
 * Get the Index values (row and column) from an electrode
 * Returns it in a integer pointer
 * ----------*/

int* DMFgui::returnIndex(QPushButton** arr,int sizerow, int sizecol, int seek){
    int val [2];
    int err [2] = {-1,-1};              //If -1,-1 is returned an error has occured
    for (int i=0;i<sizerow;i++){
        for (int j = 0; j<sizecol; j++){
            if (arr[i][j].text().toInt()==seek){
                val[0]=i;
                val[1]=j;
                return val;
            }
        }
    }
    return err;
}

/*------------
* Preview the sequence in the Serial Monitor
* Each electrode turns on one after another
* Color is set depending on the type of sequencing (sequence, mix, split,etc.)
* Preview delay is set in seconds by the user
* ----------*/

void DMFgui::on_PreviewButton_clicked()
{

    if (!mapCreated){
        mapNotCreatedErrorMessage();
    }

    else {
        QString previewElectrodes = ui->textEdit->toPlainText();                //Take the information from the Serial Monitor, may be better to set it to QString to_Send
        QStringList electrodeList = previewElectrodes.split(",");
        electrodeList.removeAt(electrodeList.length()-1);                       //Remove the last element, which is an empty space from the list
        double previewDelay = ui->PreviewSpeedEdit->text().toDouble()*1000;
        QString previewColor = "red";                                           //Default color is set to red, in case the user forgets to input a command (sequence, mix, split, etc.)
        ui->textEdit->clear();
        for (int a = 0; a<electrodeList.size(); a++){
            if(electrodeList.at(a)=="1000"){
                previewColor = "red";
                ui->textEdit->insertPlainText(electrodeList.at(a)+",");
                qApp->processEvents();
                Sleep(previewDelay);
            }
            else if (electrodeList.at(a)=="1003"){
                previewColor = "orange";
                ui->textEdit->insertPlainText(electrodeList.at(a)+",");
                qApp->processEvents();
                Sleep(previewDelay);
            }

            //Temporary solution, it only splits two electrodes, need to come up with a better solution
            else if (electrodeList.at(a)=="1001"){
                previewColor = "blue";
                int b = a+1;
                int c = a+2;
                ui->textEdit->insertPlainText(electrodeList.at(a)+",");
                qApp->processEvents();
                Sleep(previewDelay);

                ui->textEdit->insertPlainText(electrodeList.at(b)+",");
                ui->textEdit->insertPlainText(electrodeList.at(c)+",");

                //Get index of the element one positions away from 1001
                int* retIndex_1 = returnIndex(dmf_array,newrow,newcolumn,electrodeList.at(b).toInt());
                int row1 = retIndex_1[0];
                int col1 = retIndex_1[1];

                //Get index of the element two positions away from 1001
                int* retIndex_2 = returnIndex(dmf_array,newrow,newcolumn,electrodeList.at(c).toInt());
                int row2 = retIndex_2[0];
                int col2 = retIndex_2[1];

                dmf_array[row1][col1].setStyleSheet("background-color:"+previewColor+"; border-style: outset ;border-width: 2px; border-color: grey");
                dmf_array[row2][col2].setStyleSheet("background-color:"+previewColor+"; border-style: outset ;border-width: 2px; border-color: grey");
                qApp->processEvents();
                Sleep(previewDelay);
                dmf_array[row1][col1].setStyleSheet( "border-style: outset ;border-width: 2px; border-color: grey");
                dmf_array[row2][col2].setStyleSheet( "border-style: outset ;border-width: 2px; border-color: grey");
                qApp->processEvents();
                a+=2;
            }
            else{
                int* retIndex = returnIndex(dmf_array,newrow,newcolumn,electrodeList.at(a).toInt());
                int row1 = retIndex[0];
                int col1 = retIndex[1];

                ui->textEdit->insertPlainText(electrodeList.at(a)+",");

                dmf_array[row1][col1].setStyleSheet("background-color:"+previewColor+"; border-style: outset ;border-width: 2px; border-color: grey");
                qApp->processEvents();                      //Set the button to the predetermined color and Allow the command to fully process
                Sleep(previewDelay);                        //Include a delay
                dmf_array[row1][col1].setStyleSheet( "border-style: outset ;border-width: 2px; border-color: grey");
                qApp->processEvents();                      //Set the button back to the default color and Allow the command to fully process
            }
        }
    }
}

/*
 * Allow user to write directly to serial monitor
 * User should be WARNED that error may occur if the serial text is not formatted properly
 * Can be used to manually change the sequence without having to press down on a button
 * ----------*/
void DMFgui::on_WritetoTextBox_clicked(bool checked)
{
    if (checked){
        ui->textEdit->setReadOnly(false);
    }
    else
        ui->textEdit->setReadOnly(true);
}



/*-----------------------------
 *
 * Functions:
 * Create a New Map Layout
 * Saving a Map Layout
 * Loading a Map Layout
 * ------------------------------*/


/*------
 * New Map Layout, allow access to row and column text edit
 * -----*/
void DMFgui::on_newLayoutButton_clicked()
{
    openNewLayoutWindow(true);
}



void DMFgui::openNewLayoutWindow(bool checked){

    if (checked){
        newWindowButtonpressed = true;
        loadWindowButtonpressed = false;

    }
    nLayout = new newlayout();
    nLayout->exec();
    defaultNumbering = nLayout->savedNumbering;
}

/*-----------
 * Save Layout information in a list
 * Info to be stored:
 * The map size (row and column)
 * The buttons to be created (row and column)
 * ---------*/
void DMFgui::on_saveLayoutButton_clicked()
{
    if(!mapCreated){
        mapNotCreatedErrorMessage();
    }
    else {
        int z=0;
        Coordz.append(newrow);                      //The first element contains the map row size
        Coordz.append(newcolumn);                   //The second element contains the map column size
        ui->textEdit->setPlainText("");

        ui->textEdit->insertPlainText( QString::number(Coordz.at(0)) + "," + QString::number(Coordz.at(1))+ "\n");
        for (int i=0;i<newrow;i++)
        {
            for (int j=0;j<newcolumn;j++)
            {
                if(dmf_array[i][j].text()!=""){
                    z++;
                    Coordz.append(i);               //Append the the row and then column coordinates for each electrode
                    Coordz.append(j);
                }
            }
        }
        for(int p=2;p<Coordz.length();p++){
            ui->textEdit->insertPlainText(QString::number(Coordz.at(p)) + "," + QString::number(Coordz.at(p+1))+ "\n");
            p++;
        }


        //The folder in which the file will be saved to is set here
        QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save Text files"), "C:/Users/umroot/Documents/Working_DMFGUI/DMF32-K -WorkingVersion/LayoutFiles", tr("Text Files (*.txt)"));


        QFile file (fileName);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            return;
        QTextStream out(&file);
        out << ui->textEdit->toPlainText();             //**There may be a better way to send the info than through the textEdit
    }
}


/*-----------
 * Loads the map layout information from the text file
 * For now, the text file is located in the build folder
 * ---------*/

void DMFgui::on_loadLayoutButton_clicked(bool checked)
{
    if (true){
        loadWindowButtonpressed = true;
        newWindowButtonpressed = false;
    }
    lLayout = new loadlayout();
    lLayout->exec();
    defaultNumbering = lLayout->savedNumbering;

}



/*------------
 *
 * ERROR MESSAGES
 *
 * ----------*/

void DMFgui::mapNotCreatedErrorMessage()
{
    QMessageBox::warning(this,tr("Map Creation"), tr("Map is not created yet"));
}


void DMFgui::csvFileNotRead()
{
    QMessageBox::warning(this,tr("csvFile"), tr("This file has not been read yet"));
}


