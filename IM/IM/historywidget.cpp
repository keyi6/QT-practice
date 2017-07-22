#include "historywidget.h"
#include "ui_historywidget.h"

HistoryWidget::HistoryWidget(QWidget *parent, int _p1, int _p2) :
    QWidget(parent),
    p1(_p1), p2(_p2),
    ui(new Ui::HistoryWidget)
{
    ui->setupUi(this);

    setFixedSize(400, 600);
    QFont font("Microsoft Yahei");
    setFont(font);
    QIcon icon(":/new/prefix1/assets/im.png");
    setWindowIcon(icon);
    setWindowTitle("IM v1.0 © chili");

    nr = new NetworkRequest;
    allInfo = nr -> getHistory(p1, p2);

    JSON temp;
    temp["id"] = QString::number(p1);
    myName = nr -> get(temp, "getUserInfo")["username"].toString();
    temp["id"] = QString::number(p2);
    myFriendName = nr -> get(temp, "getUserInfo")["username"].toString();

    cutLen = 10, curPage = 1;
    all_page = (allInfo.length() + cutLen - 1) / cutLen;

    sa = new QScrollArea(this);
    screen = new QLabel(sa);

    sa -> setWidget(screen);
    sa -> setFixedSize(400, 560);
    sa -> setGeometry(0, 40, 400, 600);
    sa -> setStyleSheet("border: 0px;");

    screen -> setFixedWidth(370);
    screen -> setGeometry(0, 100, 400, 560);
    screen -> setMargin(20);

    prevPage = new QPushButton("<<", this);
    prevPage -> setFixedHeight(24);
    prevPage -> setGeometry(20, 6, 24, 24);
    prevPage -> setStyleSheet("background-color: #FC624D; color: #FFF;");
    connect(prevPage, SIGNAL(clicked()), this, SLOT(goPrevPage()));

    nextPage = new QPushButton(">>", this);
    nextPage -> setFixedHeight(24);
    nextPage -> setStyleSheet("background-color: #FC624D; color: #FFF;");
    nextPage -> setGeometry(50, 6, 24, 24);
    connect(nextPage , SIGNAL(clicked()), this, SLOT(goNextPage()));

    gotoPage = new QPushButton("go", this);
    gotoPage -> setFixedHeight(24);
    gotoPage -> setGeometry(300, 6, 24, 24);
    gotoPage -> setStyleSheet("background-color: #FC624D; color: #FFF;");
    connect(gotoPage, SIGNAL(clicked()), this, SLOT(clickOnGotoBtn()));

    QLabel * tempLabel = new QLabel(this);
    tempLabel -> setGeometry(360, 6, 40, 24);
    tempLabel -> setText("/" + QString::number(all_page));

    pageDisplay = new QLineEdit(this);
    pageDisplay -> setFixedSize(24, 24);
    pageDisplay -> setGeometry(330, 6, 24, 24);


    gotoThatPage(curPage);
}

void HistoryWidget::showThisPage() {
    QString temp_str = "<html><head><style>\
                       h4 {font-weight: 400; color: #000;}\
                       h5 {font-weight: 200; color: #455; font-size:12px; line-height: 14px;}\
                       p {font-weight: 100; color: #111; line-height: 20px;} \
                       </style></head><body>";

    for (int i = (curPage - 1) * cutLen; i < allInfo.size() && i < curPage * cutLen; i ++) {
        if (allInfo[i]["sender"].toInt() == p1)
            temp_str += "<h4>" + myName + "</h4>";
        else
            temp_str += "<h4>" + myFriendName + "</h4>";

        temp_str += "<h5>" + allInfo[i]["DATE_FORMAT(recTime,\"%Y-%m-%d %H:%i:%s\")"].toString()  + "</h5>" +
                    "<p>" + allInfo[i]["msg"].toString() + "<\p><br>";
    }

    screen -> setTextFormat(Qt::RichText);
    screen -> setText(temp_str + "</body>");
    screen -> adjustSize();
}

HistoryWidget::~HistoryWidget()
{
    delete ui;
}

void HistoryWidget::goNextPage() {
    if (++ curPage <= all_page)
        gotoThatPage(curPage);
    else
        -- curPage;
}

void HistoryWidget::goPrevPage() {
    if (-- curPage > 0)
        gotoThatPage(curPage);
    else
        ++ curPage;
}

void HistoryWidget::gotoThatPage(int p) {
    if (1 <= p && p <= all_page) {
        curPage = p;
        pageDisplay -> setText(QString::number(curPage));
        showThisPage();
    }
}

void HistoryWidget::clickOnGotoBtn() {
    gotoThatPage(pageDisplay -> text().toInt());
}
