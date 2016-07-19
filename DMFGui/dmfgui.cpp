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
#include <QStack>
#include <node.h>

QPushButton **dmf_array;
QGridLayout *gridLayout;
QSignalMapper *mapper;

//used for numbering the electrodes
int numberingcount = 0;                                          //electrode numbering
int reservoircount = 0;                                          //reservoir numbering
int newrow,newcolumn,resnum,corner;                              //newrow, newcolumn = for total rows and columns (including empty spaces)
                                                                 //resnum = number of reservoirs to be added
int added = 0;                                                   //keeping track of how many reservoirs were added

QString to_Send = "";                                            //Information that gets passed to the Arduino
bool enter_Button_Clicked =false;                                //ensures that user does not press the enter Button twice

//for nemesys
double flowRate;
double volume;
bool opened = false;
bool dosing = false;

bool addRes = false;                                             //true if need to keep adding reservoirs.
                                                                 //false if all reservoirs were added
//Used for Autogen path, color assignments
int* rcoord;
int* ccoord;
int track[4];
int size1;
int firstR,firstC;                                               //for clearing purposes
boolean autoGen = false;

//dimensions
int horizontalSize;
int verticalSize;

int elec = 1;                                                    //Start by adding values to the first electrode
/*
 * Structure of an electrode, array of 2 electrodes and stores their coordinates
 */
struct coordinates{
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
    ui(new Ui::DMFgui){
    ui->setupUi(this);

    arduino_is_available = false;
    arduino_port_name = "";

    ui->textEdit->setReadOnly(true);                             //not allowing user to change anything

    //for Nemesys
    ui->targetVolumeEdit->setReadOnly(true);
    ui->targetFlowRateEdit->setReadOnly(true);
    ui->unitsComboBox->addItem("nL");
    ui->unitsComboBox->addItem("μL");
    ui->unitsComboBox->addItem("mL");
    ui->unitsComboBox->addItem("L");
    ui->unitsComboBox->addItem("mm");

//      COMMENTED OUT FOR AUTOGEN
//    ui->funitscomboBox->addItem("nL/s");
//    ui->funitscomboBox->addItem("nL/min");
//    ui->funitscomboBox->addItem("μL/s");
//    ui->funitscomboBox->addItem("μL/min");
//    ui->funitscomboBox->addItem("μL/h");
//    ui->funitscomboBox->addItem("mL/min");
//    ui->funitscomboBox->addItem("mL/h");
//    ui->funitscomboBox->addItem("mm/s");

//    ui->dosingUnitscomboBox->addItem("1");
//    ui->dosingUnitscomboBox->addItem("2");
//    ui->dosingUnitscomboBox->addItem("3");

    //defining arduino
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

        arduino->setPortName(arduino_port_name);                 //open and configure the serialport
        arduino->open(QSerialPort::WriteOnly);                   //open it, only sending command, not writing anything (so writeOnly)
        arduino->setBaudRate(QSerialPort::Baud9600);             //make sure that Baud rate matches what's on the arduino
        arduino->setDataBits(QSerialPort::Data8);
        arduino->setParity(QSerialPort::NoParity);
        arduino->setStopBits(QSerialPort::OneStop);
        arduino->setFlowControl(QSerialPort::NoFlowControl);

    }
    else{                                                        //give an error message, (no arduino available)
        QMessageBox::warning(this,"Port error", "Couldn't find the Arduino!");
    }
}
DMFgui::~DMFgui(){
    if (arduino->isOpen()){
        arduino->close();
    }
    delete ui;
}
/*
 * "x" button on the main window
 */
// COMMENTED OUT FOR AUTOGEN
//void DMFgui::closeEvent(QCloseEvent *event){
//    if (opened){                                                 //close nemesys if it was opened
//        nemesys->closeConnection();
//    }
//    event -> accept();                                           //allows the main window to close
//}
/*
 * Information that gets sent to Arduino
 */
void DMFgui::save_to_String(QString electrode_num){              //#LOOK INTO STRINGSTREAM
    to_Send += electrode_num + ",";
    if (electrode_num=="1000" || electrode_num=="1001"|| electrode_num=="1003"){
        ui->textEdit->insertPlainText("\nsaved to string command: "+electrode_num + ","); //special commands.
    }
    else{                                                        //normal commands
        ui->textEdit->insertPlainText("\nsaved to string electrode number:"+electrode_num + ",");
    }
}
/*
 * Undo previous information stored in to_Send
 */
void DMFgui::on_UndoButton_clicked(){
    ui->textEdit->undo();                                        //#find a way to have this deleted one by one
}
void DMFgui::on_mixButton_clicked()
{
    save_to_String("1003");
}
void DMFgui::on_sequenceButton_clicked()
{
    save_to_String("1000");
}
void DMFgui::on_splitButton_clicked()
{
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
void DMFgui::on_sendButton_clicked(){
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,"Are you sure","Are you sure?",QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes){                              //send information if user selects "Yes"
        ui->textEdit->insertPlainText("\nSending...");

        updateDMF(to_Send);                                      //Sending information to arduino

        ui->textEdit->insertPlainText("\nSent!");
        ui->textEdit->insertPlainText(to_Send);
    }                                                            //automatically go back if user is not sure
}

/*
 * Communicating information to Arduino
 */
void DMFgui::updateDMF(QString to_Send){
    if (arduino->isWritable()){                                  //if Aruidno is available
        arduino->write(to_Send.toStdString().c_str());           //communicating with Arduino
    }
    else{
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
void DMFgui::on_enterButton_clicked(){
    QString row = ui->rowEdit->text();                           //getting information typed by user
    QString column = ui->columnEdit->text();
    int drow = row.toDouble();
    int dcolumn = column.toDouble();

    gridLayout = new QGridLayout;                                //initializing new GridLayout
    mapper = new QSignalMapper;
    QLabel display;

    gridLayout->setHorizontalSpacing(0);                         //set up of Layout
    gridLayout->setVerticalSpacing(0);
    gridLayout->setSpacing(0);

    if(!enter_Button_Clicked){                                   //goes into loop if it's the first time clicking on the enter button
        if (row.toInt() == 0||column.toInt()==0){                //value entered is not in int, pop up an error message
            QMessageBox::warning(this,tr("Not a number"), tr("This is not a number, try again"));
        }
        else{
            newrow = drow+4;                                     //adding extra space on the border for reservoirs
            newcolumn = dcolumn+4;
            horizontalSize = newcolumn;
            verticalSize = newrow;

            dmf_array = new QPushButton*[newrow];                //dynamically creating array for button storage
            for(int i=0;i<newrow;i++){
                dmf_array[i]=new QPushButton[newcolumn];
            }

            for (int i=0;i<newrow;i++){                          //naming and setting properties for each button
                for (int j=0;j<newcolumn;j++){
                    if (i==0||i==1||j==0||j==1||i==drow+3||i==drow+2||j==dcolumn+3||j==dcolumn+2){
                        dmf_array[i][j].setText("");
                        //dmf_array[i][j].setEnabled(false);
                        dmf_array[i][j].setStyleSheet("background-color:white;border:none");
                        gridLayout->addWidget(&dmf_array[i][j],i,j);
                    }
                    else{                                        //adding a new button
                        numberingcount++;
                        dmf_array[i][j].setText(QString::number(numberingcount));
                        dmf_array[i][j].setStyleSheet( "border-style: outset ;border-width: 2px; border-color: grey");
                        gridLayout->addWidget(&dmf_array[i][j],i,j); //adding button to the Layout

                        mapper->connect(&dmf_array[i][j],SIGNAL(clicked()),mapper,SLOT(map()));
                        mapper->setMapping(&dmf_array[i][j],QString::number(i)+","+QString::number(j)+","+QString::number(numberingcount));
                    }
                }
            }
                                                                 //connecting buttons to signal mapping
            connect(mapper,SIGNAL(mapped(QString)),this,SLOT(buttonClicked(QString)));
            gridLayout->addWidget(&display,newrow,0,1,newcolumn);

            ui->graphicsView->setLayout(gridLayout);             //display the DMF device

            bool ok;                                             //variable used for QInputDialog
                                                                 //adding reservoirs

            QString text = QInputDialog::getText(this,tr("Next Step"),tr("How many reservoirs do you want?"),QLineEdit::Normal,QDir::home().dirName(),&ok);
            if (text.toInt() == 0){                              //if value entered is not a number (text.toInt() will return 0)
                bool number = false;
                while (!number){                                 //while the user doesn't eneter a number, keep asking
                   QMessageBox::warning(this,tr("Not a number"), tr("This is not a number, try again"));
                   text = QInputDialog::getText(this,tr("Next Step"),tr("How many reservoirs do you want?"),QLineEdit::Normal,QDir::home().dirName(),&ok);
                   if (text.toInt() == 0){
                      number = false;
                   }
                   else{
                       number = true;
                   }
                }
            }
            if(ok&&!text.isEmpty()){
                resnum = text.toInt();                           //setting number of reservoirs to be added
                ui->textEdit->insertPlainText("please select " + text + " reservoirs");
                addRes = true;                                   //need to add the reservoirs
            }
            enter_Button_Clicked = true;                         //prevents user from clicking enter twice
        }
    }
    else{
        if (enter_Button_Clicked){                               //if the user pressed the enter button again, give warning
            QMessageBox::warning(this,tr("Invalid"), tr("You can't click on this button again"));
        }
    }
}
/*
 * Function determining what happens when a button is clicked
 * Action taken when button is clicked depends on the state of the system
 */
void DMFgui::buttonClicked(QString text){
    ClearColor();                                                //when a button is clicked, reset the color of all buttons
    QStringList electrodeList;                                   //y_coordinate,x_coordinate,electrode_number <- electrodeList
    electrodeList = text.split(",");
    if (elec ==1){                                               //saving values into the first electrode (struct)
        electrode_1.x = electrodeList.value(1).toInt();
        electrode_1.y = electrodeList.value(0).toInt();
//        ui->textEdit->insertPlainText("\n electrode_1");
//        ui->textEdit->insertPlainText("\n electrode: " +electrodeList.value(2));
//        ui->textEdit->insertPlainText("\n x coordinate: " +QString::number(electrode_1.x));
//        ui->textEdit->insertPlainText("\n y coordinate: " +QString::number(electrode_1.y));
        elec ++;                                                 //go to electrode_2 next time a button is pressed
    }
    else if(elec==2){
        electrode_2.x = electrodeList.value(1).toInt();
        electrode_2.y = electrodeList.value(0).toInt();
//        ui->textEdit->insertPlainText("\n electrode_2");
//        ui->textEdit->insertPlainText("\n electrode: " + electrodeList.value(2));
//        ui->textEdit->insertPlainText("\n x coordinate: " +QString::number(electrode_2.x));
//        ui->textEdit->insertPlainText("\n y coordinate: " +QString::number(electrode_2.y));
        elec --;                                                 //go to electrode_1 next time a button is pressed
    }
    if (addRes){                                                 //if the signal for adding a reservoir was called, add a reservoir
        if (added<resnum){
           if (add_reservoir()){
               added++;
           }
        }
        else{
            addRes = false;                                      //signal for adding reservoirs finished
            QMessageBox::warning(this,tr("Next Step"), tr("You can start generating paths"));
        }
    }
    else{                                                        //if not adding a reservoir, save to string to send to Arduino
            save_to_String(electrodeList.value(2));
    }
}
/*
 * Function for adding a reservoir
 */
bool DMFgui::add_reservoir()
{
    int * coords = getRecent_Coordinates();                      //getting most recent coordinates and saving them in a new array
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
            //location = openNewWindow(emptySpaceList);

            if (location=="top"){
                //gridLayout->addWidget(reservoir,x_coord-2,y_coord);
                setMapping(y_coord,x_coord-1);
                map_reservoir(y_coord,x_coord-2);
                return true;
                break;
            }
            if (location=="left"){
                //gridLayout->addWidget(reservoir,x_coord,y_coord-2);
                setMapping(y_coord-1,x_coord);
                map_reservoir(y_coord-2,x_coord);
                return true;
                break;
            }
            if (location=="right"){
                //gridLayout->addWidget(reservoir,x_coord,y_coord+2);
                setMapping(y_coord+1,x_coord);
                map_reservoir(y_coord+2,x_coord);
                return true;
                break;
            }
            if (location=="bottom"){
                //gridLayout->addWidget(reservoir,x_coord+2,y_coord);
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
                    //gridLayout->addWidget(reservoir,x_coord,y_coord-2);
                    setMapping(y_coord-1,x_coord);
                    map_reservoir(y_coord-2,x_coord);
                    return true;
                    break;
                }
                else if (emptySpaceList.contains("right",Qt::CaseSensitive)) //right column
                {
                    //gridLayout->addWidget(reservoir,x_coord,y_coord+2);
                    setMapping(y_coord+1,x_coord);
                    map_reservoir(y_coord+2,x_coord);
                    return true;
                    break;
                }
                else if (emptySpaceList.contains("top",Qt::CaseSensitive))
                {
                    //gridLayout->addWidget(reservoir,x_coord-2,y_coord);
                    setMapping(y_coord,x_coord-1);
                    map_reservoir(y_coord,x_coord-2);
                    return true;
                    break;
                }
                else if(emptySpaceList.contains("bottom",Qt::CaseSensitive))
                {
                    //gridLayout->addWidget(reservoir,x_coord+2,y_coord);
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
 * CLASS CALL: Dialog()
 * for choosing where the reservoir is going to be in case 2 of adding reservoirs (corner)
 */
QString DMFgui::openNewWindow(int corner)
{
//    dialog = new Dialog();
//    if (corner == 1){                                            //"top-left"
//        dialog->choice("topLeft");
//        dialog->exec();
//        return dialog->saved;
//    }
//    else if (corner == 2){                                       //"top-right"
//        dialog->choice("topRight");
//        dialog->exec();
//        return dialog->saved;
//    }
//    else if (corner == 3){                                       //"bottom-right"
//        dialog->choice("bottomRight");
//        dialog->exec();
//        return dialog->saved;
//    }
//    else if(corner == 4){                                        //"bottom-left"
//        dialog->choice("bottomLeft");
//        dialog->exec();
//        return dialog->saved;
//    }
}
/*
 * Getting most recent coordinate (either electrode_1 or electrode_2)
 */
int * DMFgui::getRecent_Coordinates(){
    int recentCoord[4];
    if (elec==1){
        recentCoord[0] = electrode_1.y;
        recentCoord[2] = electrode_2.y;
        recentCoord[1] = electrode_1.x;
        recentCoord[3] = electrode_2.x;
    }
    else if (elec==2){
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
void DMFgui::setMapping(int x, int y){
    ui->textEdit->insertPlainText("\nelectrode number:" + QString::number(numberingcount));
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
    dmf_array[y][x].setText("res no:" + QString::number(reservoircount));
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

    int a1 = z[0];                                               //Initial row
    int a2 = z[1];                                               //Initial column
    int a3 = z[2];                                               //Final row
    int a4 = z[3];                                               //Final column

    QString route = autoGeneratePath(a2,a1,a4,a3);
}
/*
 * saves most recent coordinates to the autogen path (saves beginning and end)
 */
QString DMFgui::autoGeneratePath(const int & xStart,const int & yStart,const int & xFinish,const int & yFinish){
    //node sets
    int closedNodes[horizontalSize][verticalSize];        //set of nodes that are already evaluated and marked off
    int openNodes[horizontalSize][verticalSize];          //set of nodes to be evaluated
    int dir_map[horizontalSize][verticalSize];            //map of directions(parent-child connection) keeps track of the different paths

    //directions
    const int dir = 4;
    static int dirX[dir]={1,0,-1,0}; //x direction
    static int dirY[dir]={0,1,0,-1}; //y direction

//    int currentrow = rowI;//y variable, getting called first
//    int currentcol = colI;//x variable

//    QStringList elecToCheck;

//    //for clearing purposes
//    firstR = rowI;
//    firstC = colI;

    /*
     *priority_queue: container adaptors where the first element is always the greatest of the elements
     * it contains. elements are popped from the back of the container which is known as the top of the
     * priority queue.
     *
     * this is used to get the lowest fscore
     */
//    std::priority_queue<node> pq[2];                             //defining 2 priority lists
//    std::priority_queue<node, std::vector<node>, std::function<bool(node, node)>> pq(Compare);
//    std::priority_queue<node,std::vector<node>,Compare> pq[2];

//    node min = *std::min_element(stack[index].begin(),stack[index].end());                  //lowest value = highest priority

    static int index;      //initialized to 0?                                       //static and global variables are initialized to 0
    QStack<node> stack[2];
    static node *currentNode;
    static node *neighborNode;

    //reset map
    resetMaps();
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
    activate(startNode->getyPos(),startNode->getxPos(),0);                                       //sets the start node as blue

    //push the start node to the priority queue
    stack[index].push(*startNode);

    //add to the list of open nodes in order to evaluate the start node
    openNodes[startNode->getxPos()][startNode->getyPos()] = startNode->getPriority();

    ui->textEdit->insertPlainText("\n before loop\n");

//    //while priority queue is not empty, continue
    while(!stack[index].empty()){
//    for (int j=0;j<4;j++){
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
            ui->textEdit->insertPlainText("target has been reached");
            activate(currentNode->getyPos(),currentNode->getxPos(),2);                             //activates the destination node
            //return the generated path in a QString
            QString path = "";
            int x = currentNode->getxPos();
            int y = currentNode->getyPos();
            //back track the whole procress to build a QString
            while (!(x==xStart && y ==yStart))
            {
            ui->textEdit->insertPlainText("\nxStart: "+ QString::number(xStart));
            ui->textEdit->insertPlainText("\nyStart: "+ QString::number(yStart));
            ui->textEdit->insertPlainText("\nxBefore: "+ QString::number(x));
            ui->textEdit->insertPlainText("\nyBefore: "+ QString::number(y));
                int direction = dir_map[x][y];
                path = (direction + dir/2)%dir + path; //CHECK THIS OUT
                ui->textEdit->insertPlainText("\ndirection: " + QString::number(direction));
                x += dirX[direction]; //direction is pointing towards the previous node
                y += dirY[direction];
                ui->textEdit->insertPlainText("\n\nxAfter: "+ QString::number(x));
                ui->textEdit->insertPlainText("\nyAfter: "+ QString::number(y));
                activate(y,x,1);
            }
//            //delete the node
//            delete currentNode;
//            //emptying the 'new' node
//            while (!stack[index].empty()){
//                stack[index].pop();
//            }
//            return path;
        break;
        }
        //if target hasn't been reached, add all the neighboring nodes to the OpenList + define parents for later tracing
        else{
            for (int i=0;i<dir;i++){
                /** coordinates are correct **/
                int neighborX = currentNode->getxPos()+dirX[i];
                int neighborY = currentNode->getyPos()+dirY[i];

                ui->textEdit->insertPlainText("\n neighborX:"+ QString::number(neighborX));
                ui->textEdit->insertPlainText("\n neighborY:"+ QString::number(neighborY));
                //means that it's unavailable
            if (dmf_array[neighborY][neighborX].text()==""){
                closedNodes[neighborX][neighborY]==1;
                ui->textEdit->insertPlainText("in closedNodes");
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
                    dir_map[neighborX][neighborY]=(0+dir/2)%dir;
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
                } else delete neighborNode;
            }
        }
            delete currentNode;
    } ui->textEdit->insertPlainText("\n end of loop\n");
}return ""; //no path found
}
/*
 * Resetting maps
 */
void DMFgui::resetMaps(){
//    for (int y=0;y<horizontalSize;y++){
//        for (int x=0;x<verticalSize;x++){
//            closedNodes[x][y]=0;
//            openNodes[x][y]=0;
//        }
//    }
}
/*
 * Analyzing top, bottom, left and right to the target electrode
 * returns which electrodes are available by determining if they have something
 * written on them or not
 */
QString DMFgui::findAvailableSpace(int y, int x){
    QString availElec = "";
    if (dmf_array[y-1][x].text()!=""){
        availElec += "top,";
    }
    if (dmf_array[y+1][x].text()!=""){
        availElec += "bottom,";
    }
    if (dmf_array[y][x-1].text()!=""){
        availElec += "left,";
    }
    if (dmf_array[y][x+1].text()!=""){
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
 * int position: 0=start;1=path;2=end
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
        dmf_array[firstR][firstC].setStyleSheet( "border-style: outset ;border-width: 2px; border-color: grey");
        for (int q=0;q<size1;q++){
            for(int r=0;r<size1;r++){
            dmf_array[rcoord[q]][ccoord[r]].setStyleSheet( "border-style: outset ;border-width: 2px; border-color: grey");
            }
        }
    }
    autoGen = false;
}
/*
 *Gets the F value of the A* algorithm
 */
int DMFgui::get_Fvalue(int y, int x,int rowI,int colI,int rowF, int colF){
    int f,g,h;
    ui->textEdit->insertPlainText("\n row:"+QString::number(y));
    ui->textEdit->insertPlainText("\n column:"+QString::number(x));

    //g: movement from starting point
    g=(abs(y-rowI)+abs(x-colI));
    ui->textEdit->insertPlainText("\n g value:"+QString::number(g));
    //h: movement to final destination
    h=(abs(y-rowF)+abs(x-colF));
    ui->textEdit->insertPlainText("\n h value:"+QString::number(h));
    f=g+h;
    ui->textEdit->insertPlainText("\n f value:"+QString::number(f));

    return f;
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
//COMMENTED OUT FOR AUTOGEN
//void DMFgui::on_Voltage_SendButton_clicked(){
//    QString voltage = ui->lineEdit->text();
//    float to_Send = voltage.toFloat();

//    funcgen = new funcGen();
//    funcgen->send_voltage(to_Send);
//    }
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

//COMMENTED OUT FOR AUTOGEN
//void DMFgui::nemesysNotOpenedErrorMessage(){
//    QMessageBox::warning(this,tr("NemeSYS not Opened"), tr("Please open NemeSYS first"));
//}
///*
// * display error message if nemeSYS already open
// */
//void DMFgui::nemesysAlreadyOpenedMessage(){
//    QMessageBox::warning(this,tr("NemeSYS Opened"), tr("NemeSYS is already opened"));
//}
///*
// * display error message if nemeSYS is busy
// */
//void DMFgui::nemesysDosingMessage(){
//    QMessageBox::warning(this,tr("NemeSYS is Dosing"), tr("Please wait until Dosing Unit is finished Dosing/Calibrating"));
//}
///*
// * display error message if nemeSYS is calibrating
// * #May be unnecessary, to be confirmed
// */
//void DMFgui::nemesysCalibrateMessage(){
//    QMessageBox::warning(this,tr("NemeSYS is Calibrating"), tr("Please wait until Dosing Unit is finished Calibrating"));
//}
///*
// * opening nemeSYS
// */
//void DMFgui::on_OpenButton_clicked(){
//    nemesys = new Nemesys();

//    if (!opened){
//        nemesys->openConnection();
//    }
//    else{
//        nemesysAlreadyOpenedMessage();
//    }
//    opened = true;
//}
///*
// * closing nemeSYS
// */
//void DMFgui::on_CloseButton_clicked(){
//    if (opened){
//        nemesys->closeConnection();
//        opened = false;
//    }
//    else{
//        nemesysNotOpenedErrorMessage();
//    }
//}
///*
// * dosing from syringe
// * For now, put a positive volume and negative flow
// */
//void DMFgui::on_doseButton_clicked(){
//    if (opened && nemesys->CheckDosingStatus((unsigned char)(ui->dosingUnitscomboBox->currentIndex()))== 1)// && nemesys->CheckCalibrateStatus((unsigned char)(ui->dosingUnitscomboBox->currentIndex()) == 1))
//    {
//        unsigned char dosingUnit = (unsigned char)(ui->dosingUnitscomboBox->currentIndex());        //which syringe to use

//        double volume = ui->targetVolumeEdit->text().toDouble();
//        double flow = ui->targetFlowRateEdit->text().toDouble();

//        unsigned char volumeUnit = (unsigned char)(ui->unitsComboBox->currentIndex());
//        nemesys->setActiveVolumeUnit(dosingUnit, volumeUnit);
//        unsigned char flowUnit = (unsigned char)(ui->funitscomboBox->currentIndex());
//        nemesys->setActiveFlowUnit(dosingUnit, flowUnit);

//        nemesys->DoseVolume(dosingUnit, volume,flow);
//    }
//    else if (opened && nemesys->CheckDosingStatus((unsigned char)(ui->dosingUnitscomboBox->currentIndex()))== 0){
//        nemesysDosingMessage();
//    }

//    //May be unnecessary, to be confirmed
//    else if (opened && nemesys->CheckCalibrateStatus((unsigned char)(ui->dosingUnitscomboBox->currentIndex()))== 0){
//        nemesysCalibrateMessage();
//    }
//    else if (!opened){
//        nemesysNotOpenedErrorMessage();
//    }
//}
///*
// * emptying syringe
// * For now put a positive flow value
// */
//void DMFgui::on_emptyButton_clicked()
//{
//    //Open and not dosing or calibrating
//    if (opened && nemesys->CheckDosingStatus((unsigned char)(ui->dosingUnitscomboBox->currentIndex()))== 1)    // && nemesys->CheckCalibrateStatus((unsigned char)(ui->dosingUnitscomboBox->currentIndex()) == 1))
//    {
//        unsigned char dosingUnit = (unsigned char)(ui->dosingUnitscomboBox->currentIndex());

//        double flow = ui->targetFlowRateEdit->text().toDouble();
//        unsigned char flowUnit = (unsigned char)(ui->funitscomboBox->currentIndex());
//        nemesys->setActiveFlowUnit(dosingUnit, flowUnit);

//        nemesys->EmptySyringe(dosingUnit, flow);                                                //only flow matters for emptying
//    }
//    else if (opened && nemesys->CheckDosingStatus((unsigned char)(ui->dosingUnitscomboBox->currentIndex()))== 0) {
//         nemesysDosingMessage();
//    }
//    //May be unnecessary, to be confirmed
//    else if (opened && nemesys->CheckCalibrateStatus((unsigned char)(ui->dosingUnitscomboBox->currentIndex()))== 0){
//        nemesysCalibrateMessage();
//    }
//    else if (!opened){
//         nemesysNotOpenedErrorMessage();
//    }
//}
///*
// * refilling syringe
// * For now, ensure that you have a negative flow value
// */
//void DMFgui::on_refillButton_clicked()
//{
//    //Open and not dosing or calibrating
//    if (opened && nemesys->CheckDosingStatus((unsigned char)(ui->dosingUnitscomboBox->currentIndex()))== 1)// && nemesys->CheckCalibrateStatus((unsigned char)(ui->dosingUnitscomboBox->currentIndex()) == 1))
//    {
//        unsigned char dosingUnit = (unsigned char)(ui->dosingUnitscomboBox->currentIndex());

//        double flow = ui->targetFlowRateEdit->text().toDouble();
//        unsigned char flowUnit = (unsigned char)(ui->funitscomboBox->currentIndex());
//        nemesys->setActiveFlowUnit(dosingUnit, flowUnit);

//        nemesys->RefillSyringe(dosingUnit, flow);
//    }
//    else if (opened && nemesys->CheckDosingStatus((unsigned char)(ui->dosingUnitscomboBox->currentIndex()))== 0){
//         nemesysDosingMessage();
//    }
//    //May be unnecessary, to be confirmed
//    else if (opened && nemesys->CheckCalibrateStatus((unsigned char)(ui->dosingUnitscomboBox->currentIndex()))== 0){
//        nemesysCalibrateMessage();
//    }
//    else if (!opened){
//         nemesysNotOpenedErrorMessage();
//    }
//}
///*
// * Calibrates nemeSYS
// */
//void DMFgui::on_CalibrateButton_clicked()
//{
//    //Open and not dosing
//    if (opened && nemesys->CheckDosingStatus((unsigned char)(ui->dosingUnitscomboBox->currentIndex()))== 1)// && nemesys->CheckCalibrateStatus((unsigned char)(ui->dosingUnitscomboBox->currentIndex()) == 1))
//    {

//        //Warning Message to make sure no syringes are placed
//        QMessageBox calibrateBox;
//        calibrateBox.setWindowTitle("Calibration Warning");
//        calibrateBox.setText("Please ensure that no syringes are placed in the dosing unit.\nExecuting the calibration move with a syringe fitted on the device may cause damage to the syringe.\n\nAre you ready?");
//        QAbstractButton* pButtonYes = calibrateBox.addButton(tr("Yes"), QMessageBox::YesRole);
//        calibrateBox.addButton(tr("No"), QMessageBox::NoRole);

//        calibrateBox.exec();

//        //Execute calibration if user is ready
//        if (calibrateBox.clickedButton()==pButtonYes) {
//            unsigned char dosingUnit = (unsigned char)(ui->dosingUnitscomboBox->currentIndex());

//            unsigned char volumeUnit = (unsigned char)(ui->unitsComboBox->currentIndex());
//            nemesys->setActiveVolumeUnit(dosingUnit, volumeUnit);

//            unsigned char flowUnit = (unsigned char)(ui->funitscomboBox->currentIndex());
//            nemesys->setActiveFlowUnit(dosingUnit, flowUnit);

//            nemesys->CalibrateUnit(dosingUnit);
//            nemesys->CheckCalibrateStatus(dosingUnit);
//        }
//    }
//    else if (opened && nemesys->CheckDosingStatus((unsigned char)(ui->dosingUnitscomboBox->currentIndex()))== 0){
//        nemesysDosingMessage();

//    }
//    else if (opened && nemesys->CheckCalibrateStatus((unsigned char)(ui->dosingUnitscomboBox->currentIndex()))== 0){
//        nemesysCalibrateMessage();
//    }
//    else if (!opened){
//        nemesysNotOpenedErrorMessage();
//    }

//}
///*
// * Stopping nemeSYS
// */
//void DMFgui::on_StopButton_clicked()
//{
//    if(!opened){
//        nemesysNotOpenedErrorMessage();
//    }
//    else{
//        unsigned char dosingUnit = (unsigned char)(ui->dosingUnitscomboBox->currentIndex());
//        nemesys->StopUnit(dosingUnit);
//    }
//}
///*
// * not really sure.
// */
//void DMFgui::on_MonitorButton_toggled(bool checked)
//{
//    if (checked){
//        ui->MButton->setDown(true);
//        ui->MButton->setAutoRepeat(true);
//        ui->MButton->setAutoRepeatDelay(100);
//        ui->MButton->setAutoRepeatInterval(100);
//    }
//    else{
//        ui->MButton->setAutoRepeat(false);
//        ui->MButton->setDown(false);
//    }

//}
///*
// *  Monitoring the levels of the syringes
// * updates when the Monitor button is clicked
// */
//void DMFgui::on_MButton_clicked()
//{
//    double actualFlow = nemesys->getActualFlowRate(0) ;
//    double actualSyringe = nemesys->getActualSyringeLevel(0);

//    ui->TargetMonitorFlowRate->setText(QString::number(actualFlow));
//    ui->TargetMonitorSyringeLevel->setText(QString::number(actualSyringe));
//}
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

/*
   double sminL, smaxL;
   double* sL = nemesys->getSyringeLevels(0, sminL,smaxL);
   ui->TargetMonitorFlowRate->setText(QString::number(sL[0]));
   ui->TargetMonitorSyringeLevel->setText(QString::number(sL[1]));
 */

/*--------------------------------------------------------------------------------------------------------
 *
 * For importing an image on GraphicsView
 *
--------------------------------------------------------------------------------------------------------*/

//void DMFgui::set_Scene()
//{

//    //loading the picture from computer
//    QString imagePath = QFileDialog::getOpenFileName(this,
//                                                     tr("OpenFile"),
//                                                     "",
//                                                     tr("JPEG(*.jpg *.jpeg);;PNG(*.png)")
//                                                     );

//    imageObject = new QImage();
//    imageObject->load(":/DMF_Scene.png");

//    image = QPixmap::fromImage(*imageObject);

//    scene = new QGraphicsScene(this);
//    scene->addPixmap(image);
//    scene->setSceneRect(image.rect());
//    ui->graphicsView->setScene(scene);
//    ui->graphicsView->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);

//    //manually creating a button
//    QPushButton *buttonWidget = new QPushButton;
//    buttonWidget->setMinimumWidth(200);
//    buttonWidget->setMinimumHeight(150);
//    buttonWidget->setStyleSheet("background-color: white");
//    buttonWidget->setText(QString("Test"));
//    scene->addWidget(buttonWidget)->moveBy(40,50);
//    ui->graphicsView->setScene(scene);
//}

//void DMFgui::on_loadButton_clicked()
//{
//    set_Scene();
//}