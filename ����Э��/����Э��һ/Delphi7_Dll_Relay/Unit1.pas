unit Unit1;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls;


  //--------------------------------------------------------------------
  //以下是网络继电器DLL导出的接口函数: SendCommand
  //--------------------------------------------------------------------
const
   relay_ctl_dll='RelayControlCtl.DLL';

function SendCommand(instr:PChar;outstr:PChar;outstr_len:Integer):integer;stdcall;external relay_ctl_dll name 'DeviceQuaryCommand';


   //


type
  TForm1 = class(TForm)
    Label1: TLabel;
    MemoInput: TMemo;
    Label2: TLabel;
    MemoOutput: TMemo;
    Button1: TButton;
    procedure Button1Click(Sender: TObject);
    procedure FormCreate(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form1: TForm1;

implementation

{$R *.dfm}

procedure TForm1.Button1Click(Sender: TObject);
var
   instr : array[0..1024] of byte;
   outstr : array[0..1024] of byte;
   outstr_len : integer;
   pinstr  : PChar;
   poutstr : PChar;
begin
   pinstr := PChar(MemoInput.Text);
   outstr_len   :=   SizeOf(outstr);
   poutstr := @outstr;
   ZeroMemory(poutstr,outstr_len);
   SendCommand(pinstr,poutstr,outstr_len);
   MemoOutput.Text := StrPas(poutstr);
end;

procedure TForm1.FormCreate(Sender: TObject);
begin
  MemoInput.Text := 'set_relay_val ETH:TCP:192.168.1.250:2000 R1:ON R2:ON R3:OFF';
  MemoOutput.Text := '';
end;

end.
