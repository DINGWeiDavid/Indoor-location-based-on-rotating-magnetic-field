//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MagDipoleTest.h"
#include "Rtapi.h"
#include "math.h "
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "iComponent"
#pragma link "iCustomComponent"
#pragma link "iPlot"
#pragma link "iPlotComponent"
#pragma link "iVCLComponent"
#pragma resource "*.dfm"
TMainForm *MainForm;

HANDLE hTimer1;
void RTFCNDCL Timer1Handler(PVOID Context);


//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
        : TForm(Owner)
{
//        StatusBarTCP->Panels->Items[0]->Text = "�пͻ������� ";
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ButtonConnectClick(TObject *Sender)
{
    nDataPackCount = 0;
    nDataLen = 0;
    bStartSave = false;
    nSaveDataCount = 0;
    StatusBarTCP->Panels->Items[0]->Text = "������������";

    IdUDPServer1->Active = true;
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::ButtonDisConnectClick(TObject *Sender)
{
    IdUDPServer1->Active = false;

    StatusBarTCP->Panels->Items[0]->Text = "���ӶϿ�";

}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ServerSocketSensorClientRead(TObject *Sender,
      TCustomWinSocket *Socket)
{
    char cBuf[2000];
    int nLen;


    ///��ȡ�����������ݣ� 100ms�����ݣ��ܹ�1200���ֽ�
    nLen = ServerSocketSensor->Socket->Connections[0]->ReceiveBuf(cBuf, 2000);

    if(nLen >= 1200)
    {
        nDataLen += nLen;
//        ServerSocketSensor->Socket->Connections[0]->SendBuf(cBuf, nLen);

        nDataPackCount ++;

    }

    ///������յ���õ����ݣ�����д���
    if(nLen == 1200)
        DataProcess(cBuf, 1200);
        

    LabelPackCount->Caption = IntToStr(nDataPackCount);
    LabelPackLen->Caption = IntToStr(nDataLen);


}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormCreate(TObject *Sender)
{
    nDataPackCount = 0;
    nDataLen = 0;
    bRecData = false;
}
//---------------------------------------------------------------------------
///������յ��Ĵ���������
void __fastcall TMainForm::DataProcess(char* cBuf, int nLen)
{
    MAGDATA MagData[1200];
    double fTempMagData[3][1000];
    int i;
    double fTemp;


    ////���ݽ��
    ////�����ݲ��
    memcpy(MagData, cBuf, nLen);

    ////������ת��Ϊ10���ƣ�mv)
    for(i = 0; i < 100; i++)
    {
       fTempMagData[0][i] =  (double(MagData[i].wValueX&0x00FFFFFF) *10000/8388608. - 10000.)*10.;
       fTempMagData[1][i] =  (double(MagData[i].wValueY&0x00FFFFFF) *10000/8388608. - 10000.)*10.;
       fTempMagData[2][i] =  (double(MagData[i].wValueZ&0x00FFFFFF) *10000/8388608. - 10000.)*10.;

        ///�������߸�����ʾ
       if(i%10 == 0)
       {
           SeriesX->AddY(fTempMagData[0][i]);
           SeriesY->AddY(fTempMagData[1][i]);
           SeriesZ->AddY(fTempMagData[2][i]);
       }
    }

    ///������ʾ

    LabelMagDataX->Caption = FormatFloat("0.00", fTempMagData[0][99]);
    LabelMagDataY->Caption = FormatFloat("0.00", fTempMagData[1][99]);
    LabelMagDataZ->Caption = FormatFloat("0.00", fTempMagData[2][99]);

    fTemp = sqrt(fTempMagData[0][99]*fTempMagData[0][99] + fTempMagData[1][99]*fTempMagData[1][99] + fTempMagData[2][99]*fTempMagData[2][99]);
    LabelTotalMag->Caption = FormatFloat("0.00", fTemp);




    ///������������ݴ�����ļ���
    if(bStartSave)
    {
        ////��ֵ
        for(i = 0; i < 100; i++)
        {
           fMagData[0][i + nSaveDataCount] = fTempMagData[0][i];
           fMagData[1][i + nSaveDataCount] = fTempMagData[1][i];
           fMagData[2][i + nSaveDataCount] = fTempMagData[2][i];
           fTemp = sqrt(fTempMagData[0][i]*fTempMagData[0][i] + fTempMagData[1][i]*fTempMagData[1][i] + fTempMagData[2][i]*fTempMagData[2][i]);
           fMagData[3][i + nSaveDataCount] = fTemp;
        }
        nSaveDataCount += 100;

    }

}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ButtonReceiveClick(TObject *Sender)
{
    LARGE_INTEGER liPeriod;

    ///��Ӵ����������������ʹ���
    ///������ʱ���ͻ�ȡָ������



        // ����ʵʱ��ʱ��1
    hTimer1 = RtCreateTimer(NULL,            // security
                            0,               // stack size - 0 uses default
                            Timer1Handler,   // timer handler
                            NULL,             // NULL context (argument to handler)
                            RT_PRIORITY_MAX, // priority
                            CLOCK_2);

    if(hTimer1 == 0)
    {
         CloseHandle(hTimer1);
         Application->MessageBox("����ʵʱ��ʱ��1ʧ��!","����", MB_OK + MB_ICONQUESTION);
         Application->Terminate();
     }

     ///��ʱ���Ϊ30ms
     liPeriod.QuadPart = 100 * 10000;
     if(! RtSetTimerRelative(hTimer1, &liPeriod, &liPeriod))
     {
          Application->MessageBox("���ö�ʱ��ʧ��!", "����", MB_OK + MB_ICONQUESTION);
          Application->Terminate();
     }

     IdUDPServer1->Send("192.168.10.128",10000,"start");

     bRecData = true;

}
//---------------------------------------------------------------------------
void RTFCNDCL Timer1Handler(PVOID Context)
{
    ////�ڶ�ʱ���У���ʱ������������ָ��
//    MainForm->ServerSocketSensor->Socket->Connections[0]->SendText("transform");
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Button1Click(TObject *Sender)
{
   bRecData = false;
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::ButtonSaveStartClick(TObject *Sender)
{
    int nLen = 1200;
    char Buf[2000];

    if(!bStartSave)
    {
        bStartSave  = true;
        ButtonSaveStart->Caption = "ֹͣ����";
        nSaveDataCount = 0;
    }
    else
    {
        bStartSave  = false;
        ButtonSaveStart->Caption = "��ʼ����";

        ButtonDataSave->Enabled = true;
    }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ButtonDataSaveClick(TObject *Sender)
{
    TFileStream *FileStream;
    AnsiString FullPathName, FileName, OutString;
    long I, DataCount;

    // �����ļ���Ĭ��·��
//    Screen->Cursor = crHourGlass;

    // �����ļ�
    FileName =  "C:\\temp\\MagData.m";

     if(FileExists(FileName))
    {
      FileStream = new TFileStream(FileName, fmOpenWrite);
    }
    else
    {
      FileStream = new TFileStream(FileName, fmCreate);
      FileStream->Size = 0;
    }


    try
    {
      // �����洢
//      FileStream->Position = FileStream->Size;

      for(I = 0; I < nSaveDataCount; I++)
      {
        OutString =  IntToStr(I) + "," +
                     FormatFloat("0.00", fMagData[0][I]) + " " +
                     FormatFloat("0.00", fMagData[1][I]) + " " +
                     FormatFloat("0.00", fMagData[2][I]) + " " +
                     FormatFloat("0.00", fMagData[3][I]) + " " + "\x0d"+"\x0a";

        FileStream->Write(OutString.c_str(),OutString.Length());

      }

    }
    __finally
    {
      FileStream->Free();
    }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::SeriesXAfterAdd(TChartSeries *Sender,
      int ValueIndex)
{
    Sender->GetHorizAxis->Automatic = false;
    Sender->GetHorizAxis->Maximum = Sender->XValues->MaxValue;
    Sender->GetHorizAxis->Minimum = Sender->GetHorizAxis->Maximum - 500;
//    Sender->GetHorizAxis->AutomaticMinimum = true;
//    Sender->GetHorizAxis->AutomaticMinimum = true;

}
//---------------------------------------------------------------------------

void __fastcall TMainForm::UdpSocket1Connect(TObject *Sender)
{
        StatusBarTCP->Panels->Items[0]->Text = "�пͻ���UDP���� ";
    
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::IdUDPServer1UDPRead(TObject *Sender,
      TStream *AData, TIdSocketHandle *ABinding)
{
    int nLen;
    unsigned char cBuf[2000];
    if(bRecData)
    {
        nLen = AData->Size ;

        /* ���ܵ��������Ǵ����������AData�еģ������Ƕ���rbuf��ȥ */
        AData->Read(cBuf , nLen);
        cBuf[nLen] = 0;


        if(nLen >= 1200)
        {
            nDataLen += nLen;
            nDataPackCount ++;
        }

        ///������յ���õ����ݣ�����д���
        if(nLen == 1200)
            DataProcess(cBuf, 1200);
    }

    LabelPackCount->Caption = IntToStr(nDataPackCount);
    LabelPackLen->Caption = IntToStr(nDataLen);

}
//---------------------------------------------------------------------------

