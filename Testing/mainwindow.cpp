#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <functional>
#include <QQueue>
#include <QList>
#include <QStack>
#include "object.h"
#include <QToolBar>
#include <QMenu>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QToolBar *fileToolBar;
    QMenu *fileMenu;
    QAction *actionwhatever;

    actionwhatever = new QAction(tr("&New"),this);
    actionwhatever->setShortcuts(QKeySequence::New);
    actionwhatever->setStatusTip("whetever");
    actionwhatever->setCheckable(true);

    connect(actionwhatever,SIGNAL(toggled(bool)),this,SLOT(whatever(actionwhatever)));


    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(actionwhatever);


    /** the simple queue works **/
/*    std::queue <int> q;                  //q is a queue of integers

      q.push(2);                      //put 2, 5, 3, 1 into the queue
      q.push(5);
      q.push(3);
      q.push(1);
      ui->textEdit->insertPlainText("\npq contains "+QString::number(q.size())+" elements.");

      while (!q.empty()) {
        ui->textEdit->insertPlainText(QString::number(q.front()));              //prints out 2 5 3 1
        ui->textEdit->insertPlainText(QString::number(q.back()));               //prints out 1 1 1 1
        q.pop();                      //remove the first element of the queue
      }*/

//    QQueue<int> queue;
//    queue.enqueue(1);
//    queue.enqueue(2);
//    queue.enqueue(3);
//    while (!queue.isEmpty())
//        ui->textEdit->insertPlainText(QString::number(queue.dequeue()));

//    priority_queue <int> *pq;
//    priority_queue<int,vector<int>,greate<int>> *pq;
//    pq->push(2);
//    pq->push(5);
//    pq->push(3);
//    pq->push(1);
//    ui->textEdit->insertPlainText("\npq contains "+QString::number(pq->size())+" elements.");

//    while(!pq->empty()){
//        ui->textEdit->insertPlainText(QString::number(pq->top()));       //prints out the highest priority element (5-3-2-1)
//        pq->pop();
//    }
//    QList<int> list;
//    list.append(1);
//    list.append(2);
//    list.append(3);
//    list.append(0);
//    list.append(10);

//    int min = *std::min_element(list.begin(),list.end());
//    int max = *std::max_element(list.begin(),list.end());

//    ui->textEdit->insertPlainText("\nmin: " + QString::number(min));
//    ui->textEdit->insertPlainText("\nmax: " + QString::number(max));

//    QStack<int> stack;
//    stack.push(1);
//    stack.push(2);
//    stack.push(3);
//    stack.pop();                        //takes out the most recent addition (3)
//    stack.push(0);
//    stack.push(10);
//    stack.pop();                        //takes out the most recent addition (10)

//    int min = *std::min_element(stack.begin(),stack.end());
//    int max = *std::max_element(stack.begin(),stack.end());

//     ui->textEdit->insertPlainText("\nmin: " + QString::number(min));
//     ui->textEdit->insertPlainText("\nmax: " + QString::number(max));

     QStack<Object> ostack;
     Object *som1 = new Object(1,2,4);
     Object *som2 = new Object(2,2,5);
     Object *som3 = new Object(34,7,1);
     Object *som4 = new Object(3,3,2);

//     ostack.append(something->getxPos());
//     ostack.push(something->getxPos());
     ostack.push(*som4);
     ostack.push(*som2);
     ostack.push(*som3);
     ostack.push(*som1);
//    QStack<Object>::iterator it = std::min_element(ostack.begin(),ostack.end(),compare_priority);
//     ui->textEdit->insertPlainText("\nxPos: " + QString::number(ostack.top().getPriority()));
     auto min = std::min_element( ostack.begin(), ostack.end(),
                                  []( const Object &a, const Object &b )
                                  {
                                      return a.getPriority() < b.getPriority();
                                  } );

     ui->textEdit->insertPlainText(QString::number(min->getPriority()));
}

//bool compare_priority(const Object& x, const Object& y)
//{
//    return x.getPriority() < y.getPriority();
//}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::whatever(QAction *someaction)
{
    if (someaction->isChecked()){
        ui->textEdit->insertPlainText("\n WHATEVER");
    }
    else{
        ui->textEdit->insertPlainText("\n FUCK");
    }
}
