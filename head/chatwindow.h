#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QListView>
#include <QTextEdit>
#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>

class ChatWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ChatWindow(QWidget *parent = nullptr);
    ~ChatWindow();

private:
    void initUI();    // 初始化控件和布局
    void initStyle(); // 初始化样式表

    // 左侧区域控件
    QFrame *leftFrame;
    QLabel *avatarLabel;
    QLineEdit *searchLineEdit;
    QPushButton *btnAddChat;
    QListView *conversationListView;

    // 中间区域控件
    QFrame *centerFrame;
    QLabel *chatTitleLabel;
    QPushButton *btnMore;
    QListView *messageListView;
    
    // 输入区
    QFrame *inputAreaFrame;
    QPushButton *btnEmoji;
    QPushButton *btnFile;
    QPushButton *btnScreenshot;
    QTextEdit *inputEdit;
    QPushButton *sendButton;

    // 右侧区域控件
    QFrame *rightFrame;
    QLabel *rightTitleLabel;
};

#endif // CHATWINDOW_H