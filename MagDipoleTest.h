//---------------------------------------------------------------------------

#ifndef MagDipoleTestH
#define MagDipoleTestH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Chart.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Series.hpp>
#include <TeEngine.hpp>
#include <TeeProcs.hpp>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdTCPServer.hpp>
#include <ScktComp.hpp>
#include "iComponent.hpp"
#include "iCustomComponent.hpp"
#include "iPlot.hpp"
#include "iPlotComponent.hpp"
#include "iVCLComponent.hpp"
#include <Sockets.hpp>
#include <IdUDPBase.hpp>
#include <IdUDPServer.hpp>



typedef struct tag_MAGDATA
{
    DWORD wValueX;
    DWORD wValueY;
    DWORD wValueZ;
}MAGDATA;




//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:	// IDE-managed Components
        TPanel *Panel1;
        TLabel *Label1;
        TButton *ButtonConnect;
        TStatusBar *StatusBarTCP;
        TButton *ButtonDisConnect;
        TButton *ButtonReceive;
        TButton *ButtonSaveStart;
        TButton *ButtonDataSave;
        TGroupBox *GroupBox1;
        TLabel *Label2;
    TLabel *LabelMagDataX;
        TLabel *Label4;
    TLabel *LabelMagDataY;
        TLabel *Label6;
    TLabel *LabelMagDataZ;
    TLabel *LabelTotalMag;
        TLabel *Label9;
    TServerSocket *ServerSocketSensor;
    TGroupBox *GroupBox2;
    TLabel *Label10;
    TLabel *LabelPackCount;
    TLabel *Label12;
    TLabel *LabelPackLen;
    TButton *Button1;
    TChart *Chart1;
    TFastLineSeries *SeriesX;
    TChart *Chart2;
    TFastLineSeries *SeriesY;
    TPanel *Panel2;
    TChart *Chart3;
    TFastLineSeries *SeriesZ;
    TUdpSocket *UdpSocket1;
    TIdUDPServer *IdUDPServer1;
    void __fastcall ButtonConnectClick(TObject *Sender);
    void __fastcall ButtonDisConnectClick(TObject *Sender);
    void __fastcall ServerSocketSensorClientRead(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall ButtonReceiveClick(TObject *Sender);
    void __fastcall Button1Click(TObject *Sender);
    void __fastcall ButtonSaveStartClick(TObject *Sender);
    void __fastcall ButtonDataSaveClick(TObject *Sender);
    void __fastcall SeriesXAfterAdd(TChartSeries *Sender, int ValueIndex);
    void __fastcall UdpSocket1Connect(TObject *Sender);
    void __fastcall IdUDPServer1UDPRead(TObject *Sender, TStream *AData,
          TIdSocketHandle *ABinding);
private:	// User declarations
public:		// User declarations
        __fastcall TMainForm(TComponent* Owner);

        void __fastcall DataProcess(char* cBuf, int nLen);

        long nDataPackCount;
        long nDataLen;

        BOOL bStartSave;

        BOOL bRecData;
        long nSaveDataCount;
        double fMagData[4][1000000];
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
