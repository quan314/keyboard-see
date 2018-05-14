#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QApplication>
#include <Windows.h>
#include <QDebug>
#include <QSettings>
#include <QStringListModel>
#include<QStringList>

//��ע����Ŀ¼������������
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
  ���õ���dll�Ϳ���ִ�е�ȫ�ֹ���ʾ��:
  ֻ����F12��Ϊ�����˳�,���μ��̺����,����ʱ��ע��
  ********************************************************/

HHOOK keyHook=NULL;
HHOOK mouseHook=NULL;
Ui::MainWindow *uii;
WPARAM lastkey=0;
QString lastStr;

//���̹��ӹ���
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

            if ( wParam == WM_KEYDOWN ) //�м�����
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
               uii->textEdit->append(now);       //���������Ϣ��ע�����ﰴ�º͵��𶼻����
            }
        }

    //��WH_KEYBOARD_LLģʽ��lParam ��ָ��KBDLLHOOKSTRUCT���͵�ַ
    KBDLLHOOKSTRUCT *pkbhs = (KBDLLHOOKSTRUCT *) lParam;
    //���nCode����HC_ACTION�������Ϣ�����С��0�������ӳ̾ͱ��뽫����Ϣ���ݸ� CallNextHookEx

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

        //return 1;//����1��ʾ��ȡ��Ϣ���ٴ���,����0��ʾ��������,��Ϣ��������
         return CallNextHookEx(keyHook, nCode, wParam, lParam);
    }*/
    return CallNextHookEx(keyHook, nCode, wParam, lParam);
}
//��깳�ӹ���
LRESULT CALLBACK mouseProc(int nCode,WPARAM wParam,LPARAM lParam )
{

    return 1;

}
//ж�ع���
void unHook()
{
    UnhookWindowsHookEx(keyHook);
  //  UnhookWindowsHookEx(mouseHook);

}
//��װ����,���øú�������װ����
void setHook(Ui::MainWindow *ui)
{
    //�������ײ㹳��,��ҪDLL�Ϳ���ȫ��
    //�ײ���̹���
    keyHook =SetWindowsHookEx( WH_KEYBOARD_LL,keyProc,GetModuleHandle(NULL),0);

    uii=ui;

    //�ײ���깳��
//    mouseHook =SetWindowsHookEx( WH_MOUSE_LL,mouseProc,GetModuleHandle(NULL),0);
}

//���ע������Ctrl+Alt+del
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
    //ע�� ����
    setHook(this->ui);
    qDebug() << "0000000";
}


void MainWindow::on_pushButton_2_clicked()
{
    // ���� ����
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
    if(ui->pushButton_5->text() == QString::fromLocal8Bit("����������")){
        task=FindWindow(L"Shell_TrayWnd",NULL);
        ShowWindow(task,SW_HIDE);
        ui->pushButton_5->setText(QString::fromLocal8Bit("��ʾ������"));
    }else{
        ShowWindow(task,SW_SHOW);
        ui->pushButton_5->setText(QString::fromLocal8Bit("����������"));
    }
}
