#include "chatwindow.h"

ChatWindow::ChatWindow(QWidget *parent)
    : QWidget(parent)
{
    this->resize(1000, 700);
    this->setWindowTitle("WeChat Style Chat");
    
    initUI();
    initStyle();
}

ChatWindow::~ChatWindow()
{
}

void ChatWindow::initUI()
{
    // === 主布局 ===
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // ==========================================
    // 1. 左侧侧边栏 (Left Frame)
    // ==========================================
    leftFrame = new QFrame(this);
    leftFrame->setObjectName("leftFrame"); // 用于QSS定位
    leftFrame->setFixedWidth(280); // 固定宽度

    QVBoxLayout *leftLayout = new QVBoxLayout(leftFrame);
    leftLayout->setContentsMargins(12, 20, 12, 0);
    leftLayout->setSpacing(10);

    // 1.1 顶部头像与搜索
    QWidget *leftHeader = new QWidget(leftFrame);
    QHBoxLayout *leftHeaderLayout = new QHBoxLayout(leftHeader);
    leftHeaderLayout->setContentsMargins(0, 0, 0, 0);
    leftHeaderLayout->setSpacing(10);

    avatarLabel = new QLabel("Me", leftHeader);
    avatarLabel->setFixedSize(36, 36);
    avatarLabel->setAlignment(Qt::AlignCenter);
    avatarLabel->setObjectName("avatarLabel"); // 样式单独处理

    searchLineEdit = new QLineEdit(leftHeader);
    searchLineEdit->setPlaceholderText(QStringLiteral("搜索"));
    searchLineEdit->setFixedHeight(28);

    btnAddChat = new QPushButton("+", leftHeader);
    btnAddChat->setFixedSize(28, 28);
    btnAddChat->setObjectName("btnAddChat");

    leftHeaderLayout->addWidget(avatarLabel);
    leftHeaderLayout->addWidget(searchLineEdit);
    leftHeaderLayout->addWidget(btnAddChat);

    // 1.2 会话列表
    conversationListView = new QListView(leftFrame);
    conversationListView->setFrameShape(QFrame::NoFrame);

    leftLayout->addWidget(leftHeader);
    leftLayout->addWidget(conversationListView);


    // ==========================================
    // 2. 中间聊天主区域 (Center Frame)
    // ==========================================
    centerFrame = new QFrame(this);
    centerFrame->setObjectName("centerFrame");

    QVBoxLayout *centerLayout = new QVBoxLayout(centerFrame);
    centerLayout->setContentsMargins(0, 0, 0, 0);
    centerLayout->setSpacing(0);

    // 2.1 聊天标题栏
    QWidget *chatHeader = new QWidget(centerFrame);
    chatHeader->setFixedHeight(50);
    chatHeader->setObjectName("chatHeader"); // 用于画底边框
    
    QHBoxLayout *headerLayout = new QHBoxLayout(chatHeader);
    headerLayout->setContentsMargins(20, 0, 20, 0);

    chatTitleLabel = new QLabel(QStringLiteral("文件传输助手"), chatHeader);
    chatTitleLabel->setObjectName("chatTitle");

    btnMore = new QPushButton("...", chatHeader);
    btnMore->setFlat(true);
    btnMore->setObjectName("btnMore");
    btnMore->setFixedWidth(30);

    headerLayout->addWidget(chatTitleLabel);
    headerLayout->addStretch();
    headerLayout->addWidget(btnMore);

    // 2.2 消息列表
    messageListView = new QListView(centerFrame);
    messageListView->setObjectName("messageListView");
    messageListView->setFrameShape(QFrame::NoFrame);
    messageListView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

    // 2.3 输入区域
    inputAreaFrame = new QFrame(centerFrame);
    inputAreaFrame->setObjectName("inputAreaFrame");
    inputAreaFrame->setFixedHeight(200); // 给定一个初始高度

    QVBoxLayout *inputLayout = new QVBoxLayout(inputAreaFrame);
    inputLayout->setContentsMargins(15, 10, 15, 10);
    inputLayout->setSpacing(5);

    // 2.3.1 工具栏
    QWidget *toolBar = new QWidget(inputAreaFrame);
    QHBoxLayout *toolBarLayout = new QHBoxLayout(toolBar);
    toolBarLayout->setContentsMargins(0, 0, 0, 0);
    toolBarLayout->setSpacing(15);

    // 创建工具栏按钮 helper
    auto createToolBtn = [](const QString &text) {
        QPushButton *btn = new QPushButton(text);
        btn->setFlat(true);
        btn->setFixedWidth(24);
        btn->setStyleSheet("border: none; font-size: 18px; color: #666;");
        return btn;
    };

    btnEmoji = createToolBtn(QStringLiteral("☺"));
    btnFile = createToolBtn(QStringLiteral("📁"));
    btnScreenshot = createToolBtn(QStringLiteral("✂"));

    toolBarLayout->addWidget(btnEmoji);
    toolBarLayout->addWidget(btnFile);
    toolBarLayout->addWidget(btnScreenshot);
    toolBarLayout->addStretch();

    // 2.3.2 输入框
    inputEdit = new QTextEdit(inputAreaFrame);
    inputEdit->setFrameShape(QFrame::NoFrame);
    inputEdit->setPlaceholderText("");

    // 2.3.3 发送按钮区域
    QWidget *sendArea = new QWidget(inputAreaFrame);
    QHBoxLayout *sendAreaLayout = new QHBoxLayout(sendArea);
    sendAreaLayout->setContentsMargins(0, 0, 0, 0);
    
    sendButton = new QPushButton(QStringLiteral("发送(S)"), sendArea);
    sendButton->setFixedSize(90, 30);
    sendButton->setObjectName("sendButton");

    sendAreaLayout->addStretch();
    sendAreaLayout->addWidget(sendButton);

    inputLayout->addWidget(toolBar);
    inputLayout->addWidget(inputEdit);
    inputLayout->addWidget(sendArea);

    centerLayout->addWidget(chatHeader);
    centerLayout->addWidget(messageListView);
    centerLayout->addWidget(inputAreaFrame);

    // ==========================================
    // 3. 右侧资料栏 (Right Frame)
    // ==========================================
    rightFrame = new QFrame(this);
    rightFrame->setObjectName("rightFrame");
    rightFrame->setFixedWidth(0); // 默认折叠，如果要显示改成 250
    // 如果你想默认显示右边栏，注释掉上面一行，解开下面一行
    // rightFrame->setFixedWidth(240); 

    QVBoxLayout *rightLayout = new QVBoxLayout(rightFrame);
    rightLayout->setContentsMargins(0, 20, 0, 0);
    
    rightTitleLabel = new QLabel(QStringLiteral("资料 / 文件"), rightFrame);
    rightTitleLabel->setAlignment(Qt::AlignCenter);
    rightTitleLabel->setStyleSheet("color: #999; font-size: 12px;");

    rightLayout->addWidget(rightTitleLabel);
    rightLayout->addStretch();

    // === 添加到主布局 ===
    mainLayout->addWidget(leftFrame);
    mainLayout->addWidget(centerFrame);
    mainLayout->addWidget(rightFrame);
}

void ChatWindow::initStyle()
{
    // 使用原始字符串字面量 R"(...)" 定义样式表，避免转义烦恼
    QString qss = R"(
        /* 全局字体 */
        * {
            font-family: "Microsoft YaHei", "Segoe UI", sans-serif;
            font-size: 14px;
            color: #333;
        }

        /* 左侧背景 */
        QFrame#leftFrame {
            background-color: #ececec; /* 微信浅灰 */
            border-right: 1px solid #dcdcdc;
        }

        /* 搜索框 */
        QLineEdit {
            background-color: #d9d9d9;
            border: 1px solid transparent;
            border-radius: 4px;
            padding: 4px;
            font-size: 12px;
        }
        QLineEdit:focus {
            background-color: #fff;
            border: 1px solid #b2b2b2;
        }

        /* 头像模拟 */
        QLabel#avatarLabel {
            background-color: #4a90e2;
            border-radius: 4px;
            color: white;
            font-weight: bold;
        }

        /* 添加按钮 */
        QPushButton#btnAddChat {
            background-color: #d9d9d9;
            border-radius: 4px;
            color: #666;
            border: none;
        }
        QPushButton#btnAddChat:hover {
            background-color: #d0d0d0;
        }

        /* 左侧列表 */
        QListView {
            background-color: #ececec;
            border: none;
            outline: none;
        }
        QListView::item {
            height: 60px;
            padding-left: 10px;
        }
        QListView::item:selected {
            background-color: #c6c6c6;
            color: #333;
        }
        QListView::item:hover {
            background-color: #dcdcdc;
        }

        /* 中间聊天背景 */
        QFrame#centerFrame {
            background-color: #f5f5f5;
        }

        /* 标题栏 */
        QWidget#chatHeader {
            border-bottom: 1px solid #e7e7e7;
        }
        QLabel#chatTitle {
            font-size: 16px;
            font-weight: bold;
        }
        QPushButton#btnMore {
            font-size: 16px; 
            color: #666; 
            border: none; 
            background: transparent;
        }
        QPushButton#btnMore:hover {
            background-color: #e0e0e0;
            border-radius: 4px;
        }

        /* 消息区背景 */
        QListView#messageListView {
            background-color: #f5f5f5;
            border: none;
        }

        /* 输入区背景 */
        QFrame#inputAreaFrame {
            background-color: #fff;
            border-top: 1px solid #e7e7e7;
        }

        /* 发送按钮 (微信风格) */
        QPushButton#sendButton {
            background-color: #f5f5f5;
            color: #606060;
            border: 1px solid #e5e5e5;
            border-radius: 4px;
        }
        QPushButton#sendButton:hover {
            background-color: #129611; /* 微信绿 */
            color: white;
            border: none;
        }

        /* 右侧栏 */
        QFrame#rightFrame {
            background-color: #fff;
            border-left: 1px solid #e7e7e7;
        }
    )";

    this->setStyleSheet(qss);
}
