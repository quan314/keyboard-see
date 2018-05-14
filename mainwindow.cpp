#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QApplication>
#include <Windows.h>
#include <QDebug>
#include <QSettings>
#include <QStringListModel>
#include<QStringList>

//在注册表该目录下增加新内容
#define TASKMANAGER "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QStringList  num;

}

MainWindow::~MainWindow()
{
    delete ui;
}

/**********************************************************
  不用调用dll就可以执行的全局钩子示例:
  只留下F12做为程序退出,屏蔽键盘和鼠标,测试时请注意
  ********************************************************/

HHOOK keyHook=NULL;
HHOOK mouseHook=NULL;
Ui::MainWindow *uii;
WPARAM lastkey=0;
QString lastStr;

//键盘钩子过程
LRESULT CALLBACK keyProc(int nCode,WPARAM wParam,LPARAM lParam )
{

   KBDLLHOOKSTRUCT *pkbhs = (KBDLLHOOKSTRUCT *) lParam;
   if( pkbhs->flags & LLKHF_ALTDOWN){

   }

    if (nCode >= HC_ACTION && (wParam == WM_KEYDOWN) || (wParam == WM_KEYUP))
        {
        bool fflag=0;
        if (lastkey == wParam)
            {
                uii->textEdit->append("&&");
                fflag=1;
            }
            lastkey = wParam;

            if ( wParam == WM_KEYDOWN ) //有键按下
            {
                DWORD vk_code = ((KBDLLHOOKSTRUCT*)lParam)->vkCode;
                //printf("lParam = %d code = %d HC_ACTION = %d WM_KEYDOWN=%d wParam = %d vk_code = %d\n", l, code, HC_ACTION, WM_KEYDOWN, w, vk_code);
                //printf("%d##%c", vk_code, vk_code);
                BYTE ks[256];
                GetKeyboardState(ks);
                WORD w;
                ToAscii(vk_code, 0, ks, &w, 0);
                char ch = char(w);
                log(ch);
                QString now  = (QChar)ch;
                if(fflag==1)
                    now=lastStr+now;
                lastStr=now;
               uii->textEdit->append(now);       //输出按键信息，注意这里按下和弹起都会输出
            }
        }

    //在WH_KEYBOARD_LL模式下lParam 是指向KBDLLHOOKSTRUCT类型地址
    KBDLLHOOKSTRUCT *pkbhs = (KBDLLHOOKSTRUCT *) lParam;
    //如果nCode等于HC_ACTION则处理该消息，如果小于0，则钩子子程就必须将该消息传递给 CallNextHookEx

    /*if(nCode == HC_ACTION){
        if(pkbhs->vkCode == VK_ESCAPE && GetAsyncKeyState(VK_CONTROL)& 0x8000 && GetAsyncKeyState(VK_SHIFT)&0x8000){
                    qDebug() << "Ctrl+Shift+Esc";
        }else if(pkbhs->vkCode == VK_ESCAPE && GetAsyncKeyState(VK_CONTROL) & 0x8000){
            qDebug() << "Ctrl+Esc";
        }else if(pkbhs->vkCode == VK_TAB && pkbhs->flags & LLKHF_ALTDOWN){
            qDebug() << "Alt+Tab";
        }else if(pkbhs->vkCode == VK_ESCAPE && pkbhs->flags &LLKHF_ALTDOWN){
            qDebug() << "Alt+Esc";
        }else if(pkbhs->vkCode == VK_LWIN || pkbhs->vkCode == VK_RWIN){
            qDebug() << "LWIN/RWIN";
        }else if(pkbhs->vkCode == VK_F4 && pkbhs->flags & LLKHF_ALTDOWN){
            qDebug() << "Alt+F4";
        }
        if(GetAsyncKeyState(VK_CONTROL) & 0x8000)
            uii->textEdit->append("ctrl");
        else if(pkbhs->flags & LLKHF_ALTDOWN)
             uii->textEdit->append("Alt");
        else{
            BYTE ks[256];
                        GetKeyboardState(ks);
                        WORD w;
                        ToAscii(pkbhs->vkCode, 0, ks, &w, 0);
                        char ch = char(w);
                        log(ch);
                      QString s = (QChar)ch;
            uii->textEdit->append(s);
        }


        if(pkbhs->vkCode == VK_F12)
        {
            void unHook();
            qApp->quit();
        }

        //return 1;//返回1表示截取消息不再传递,返回0表示不作处理,消息继续传递
         return CallNextHookEx(keyHook, nCode, wParam, lParam);
    }*/
    return CallNextHookEx(keyHook, nCode, wParam, lParam);
}
//鼠标钩子过程
LRESULT CALLBACK mouseProc(int nCode,WPARAM wParam,LPARAM lParam )
{

    return 1;

}
//卸载钩子
void unHook()
{
    UnhookWindowsHookEx(keyHook);
  //  UnhookWindowsHookEx(mouseHook);

}
//安装钩子,调用该函数即安装钩子
void setHook(Ui::MainWindow *ui)
{
    //这两个底层钩子,不要DLL就可以全局
    //底层键盘钩子
    keyHook =SetWindowsHookEx( WH_KEYBOARD_LL,keyProc,GetModuleHandle(NULL),0);

    uii=ui;

    //底层鼠标钩子
//    mouseHook =SetWindowsHookEx( WH_MOUSE_LL,mouseProc,GetModuleHandle(NULL),0);
}

//添加注册评比Ctrl+Alt+del
void TaskManager(bool flag)
{
    QSettings *settings = new QSettings(TASKMANAGER, QSettings::NativeFormat);
    if(flag){
        settings->setValue("DisableTaskMgr", "1");
    }else{
        settings->remove("DisableTaskMgr");
    }
    delete settings;
}

void MainWindow::on_pushButton_clicked()
{
    //注册 钩子
    setHook(this->ui);
    qDebug() << "0000000";
}


void MainWindow::on_pushButton_2_clicked()
{
    // 销毁 钩子
    unHook();
    qDebug() << "33333333";
}

void MainWindow::on_pushButton_3_clicked()
{
    TaskManager(true);
}

void MainWindow::on_pushButton_4_clicked()
{
    TaskManager(false);
}

void MainWindow::on_pushButton_5_clicked()
{
    if(ui->pushButton_5->text() == QString::fromLocal8Bit("隐藏任务栏")){
        task=FindWindow(L"Shell_TrayWnd",NULL);
        ShowWindow(task,SW_HIDE);
        ui->pushButton_5->setText(QString::fromLocal8Bit("显示任务栏"));
    }else{
        ShowWindow(task,SW_SHOW);
        ui->pushButton_5->setText(QString::fromLocal8Bit("隐藏任务栏"));
    }
}
