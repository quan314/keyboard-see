#include "keyhookvs.h"
#include <QApplication>
#include <Windows.h>
#include <QDebug>
#include <QSettings>
#include <QStringListModel>
#include<QStringList>
#include <Psapi.h>
#include<process.h>
#pragma comment (lib,"Psapi.lib")

#define TASKMANAGER "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System"


keyhookvs::keyhookvs(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
}
int otherchar(int code)
{

    if(code==189||code==187||code==219||code==221||code==19
     ||code==220||code==186||code==222||code==188||
            code==190||code==191||(code>=96&&code<=110))
        return 1;
    else return 0;
}

 QString noChar(int code)
 {
     QString c;
     if(code>=112&&code<=123)
     {
         c="F"+QString::number(code-111);
     }
     else{
     switch (code) {
     case 27:
         c="Esc";
         break;
     case 144:
         c="NumLock";
         break;
     case 13:
         c="Enter";
         break;
     case 46:
         c="Delete";
         break;
     case 33:
         c="PageUp";
         break;
     case 34:
         c="PageDown";
         break;
     case 35:
         c="End";
         break;
     case 36:
         c="Home";
         break;
     case 37:
         c="left";
         break;
     case 38:
         c="up";
         break;
     case 39:
         c="right";
         break;
     case 40:
         c="down";
         break;
     case 9:
         c="Tab";
         break;
     case 20:
         c="CapsLock";
         break;
     case 160:
         c="Shift(l)";
         break;
     case 161:
         c="Shift(r)";
         break;
     case 162:
         c="Ctrl(l)";
         break;
     case 163:
         c="Ctrl(r)";
         break;
     case 91:
         c="Win(l)";
         break;
     case 92:
         c="Win(r)";
         break;
     case 164:
         c="Alt(l)";
         break;
     case 165:
         c="Alt(r)";
         break;
     case 8:
         c="Backspace";
         break;
     case 32:
         c="SpaceBar";
         break;
     default:
         c="*";
         break;
     }
     }
     return c;
 }

 QString orherPoint(DWORD vk_code)
 {
	 QString nn;
	 switch (vk_code) {
	 case 19: nn = "Numlock";
		 break;
	 case 189: nn = "-";
		 break;
	 case 187: nn = "=";
		 break;
	 case 219: nn = "[";
		 break;
	 case 221: nn = "]";
		 break;
	 case 220: nn = "\\";
		 break;
	 case 186: nn = ";";
		 break;
	 case 222: nn = "'";
		 break;
	 case 188: nn = ",";
		 break;
	 case 190: nn = ".";
		 break;
	 case 191: nn = "/";
		 break;
	 case 192: nn = "`";
		 break;
	 case 107: nn = "+";
		 break;
	 case 109: nn = "-";
		 break;
	 case 106: nn = "*";
		 break;
	 case 111: nn = "//";
		 break;

	 default:
		 break;
	 }
	 return nn;
 }


HHOOK keyHook=NULL;
HHOOK mouseHook=NULL;
Ui::keyhookvsClass *uii;
WPARAM lastkey=0;
QString lastStr;
int pr=0;
int altflag = 0;
int AltDown(KBDLLHOOKSTRUCT *pkbhs)
{
	if (altflag == 0)
	{
		if ((pkbhs->flags & LLKHF_ALTDOWN) )
		{
			return 1;
		}
		return 0;
	}
	return 0;
}

//键盘钩子过程
LRESULT CALLBACK keyProc(int nCode,WPARAM wParam,LPARAM lParam )
{

   KBDLLHOOKSTRUCT *pkbhs = (KBDLLHOOKSTRUCT *) lParam;
     QString now="";
	/* if (pr >= 5&&pr!=100)
	 {
		 qDebug() << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
		 pr = 0;
	 }*/
   if ((nCode >= HC_ACTION && (wParam == WM_KEYDOWN) || (wParam == WM_KEYUP))
	   //|| (pkbhs->flags & LLKHF_ALTDOWN)
	  ||( wParam == WM_SYSKEYDOWN)||(wParam == WM_SYSKEYUP)
       //    ||(GetAsyncKeyState(VK_CONTROL) & 0x8000)
            )
        {
	   if (pr == 100)
		   pr = 0;
        bool fflag;
		
		/*257up   256down*/
        if (((lastkey == WM_KEYDOWN||lastkey== WM_SYSKEYDOWN)&& (wParam == WM_KEYDOWN || wParam== WM_SYSKEYDOWN)) ||lastkey==0)
            {
               // uii->textEdit->append("&&");
                fflag=1;
            }
		else fflag = 0;
		qDebug() << "lastkey" << lastkey << "wparam" << wParam;
        lastkey = wParam;
		//if ((pkbhs->flags & LLKHF_ALTDOWN))
			//fflag = 1;
		

            if ( wParam == WM_KEYUP)
           {
             
				if (pr > 0)
			   {
				   pr--;
				   
			   }
           }


            if ( wParam == WM_KEYDOWN
				//|| (pkbhs->flags & LLKHF_ALTDOWN)
				|| (wParam == WM_SYSKEYDOWN)
				//||AltDown(pkbhs)
               //  ||(GetAsyncKeyState(VK_CONTROL) & 0x8000)
                 ) //有键按下
            {

				
					pr++;
			
					if (wParam == WM_KEYDOWN)
						qDebug() << "111111111";
					else qDebug() << "2222222222";
				
				
				DWORD vk_code = ((KBDLLHOOKSTRUCT*)lParam)->vkCode;
                
                BYTE ks[256];
                GetKeyboardState(ks);
                WORD w;
                if(GetKeyState(VK_CONTROL)&0x8000)
                {
                    qDebug()<< "CCCC"<<vk_code;

                }
                ToAscii(vk_code, 0, ks, &w, 0);
                char ch = char(w);
                qDebug()<< "w"<<w<<endl;
                 qDebug()<<"vk-code"<<QString::number(vk_code);
                log(ch);


                if((vk_code>=48&&vk_code<=90)||otherchar(vk_code))
                {
                     now  = (QChar)ch;
                     if(GetKeyState(VK_CONTROL)&0x8000)
                     {
                        if(otherchar(vk_code)==1)
                        {
                            QString nn;
                           /* switch (vk_code) {
                            case 19: nn="Numlock";
                                break;
                            case 189: nn="-";
                                break;
                            case 187: nn="=";
                                break;
                            case 219: nn="[";
                                break;
                            case 221: nn="]";
                                break;
                            case 220: nn="\\";
                                break;
                            case 186: nn=";";
                                break;
                            case 222: nn="'";
                                break;
                            case 188: nn=",";
                                break;
                            case 190: nn=".";
                                break;
                            case 191: nn="/";
                                break;
                            case 192: nn="`";
                                break;
                            case 107: nn="+";
                                break;
                            case 109: nn="-";
                                break;
                            case 106: nn="*";
                                break;
                            case 111: nn="//";
                                break;

                            default:
                                break;
                            }*/
							now = otherchar(vk_code);
                           // now=nn;

                        }
                        else
                         now=vk_code;
                     }
                    


                }
                else
                {

					if ((pkbhs->flags & LLKHF_ALTDOWN) == 1)
						now = "alt";
					else
					now=noChar(vk_code);

                }
				qDebug() << "now" << now;
               // QString now = QString::number(vk_code);

                if(/*(pr>0&&pr!=100)||*/ fflag == 1)
                {now=lastStr+now;
                 
                }
                else{

                    lastStr="";

                }
                lastStr=now+" ";

                          //uii->textEdit->append(now);
                      //输出按键信息，注意这里按下和弹起都会输出
				
            }
			qDebug() << "last" << lastStr<<"ffalg "<<fflag;
		
			if ((pkbhs->flags & LLKHF_ALTDOWN) == 0)
				altflag = 0;
			if ((pkbhs->flags & LLKHF_ALTDOWN)!=0)
			{
				altflag = 1;
			}
			if(((wParam == WM_KEYUP )|| (wParam == WM_SYSKEYDOWN))&&fflag==0)
             {
				uii->textEdit->append(lastStr);
              pr=100;
			  now = "";
			  lastStr = "";
			  lastkey = 0;
			 
            }

        }


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
    keyHook=NULL;
  //  UnhookWindowsHookEx(mouseHook);

}
//安装钩子,调用该函数即安装钩子
void setHook(Ui::keyhookvsClass *ui)
{
    //这两个底层钩子,不要DLL就可以全局
    //底层键盘钩子
    if(keyHook==NULL)
    {
        keyHook =SetWindowsHookEx( WH_KEYBOARD_LL,keyProc,GetModuleHandle(NULL),0);
    }
    qDebug()<<"keyHOOK"<<keyHook;
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



void keyhookvs::on_pushButton_clicked()
{
    //注册 钩子


    setHook(&(this->ui));
    qDebug() << "0000000";
    lastStr="";
}


void keyhookvs::on_pushButton_2_clicked()
{
    // 销毁 钩子
    unHook();
    qDebug() << "33333333";
}

void keyhookvs::on_pushButton_3_clicked()
{
    TaskManager(true);
}

void keyhookvs::on_pushButton_4_clicked()
{
    TaskManager(false);
}

void keyhookvs::on_pushButton_5_clicked()
{
   
	if(ui.pushButton_5->text() == QString::fromLocal8Bit("隐藏任务栏")){
        task=FindWindow(L"Shell_TrayWnd",NULL);
        ShowWindow(task,SW_HIDE);
        ui.pushButton_5->setText(QString::fromLocal8Bit("显示任务栏"));
    }else{
        ShowWindow(task,SW_SHOW);
        ui.pushButton_5->setText(QString::fromLocal8Bit("隐藏任务栏"));
    }
}
