
Public Class Form1




    Private Declare Function DeviceQuaryCommand Lib "RelayControlCtl.dll" (ByVal quary_str As String, ByVal out As String, ByVal out_len As Integer) As Integer

    Private Sub Form1_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        RichTextBox1.Text = "set_relay_val ETH:TCP:192.168.1.250:2000 R1:ON R2:ON  R3:OFF"
    End Sub

    Private Sub Button1_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button1.Click
        Dim y As String
        '���¶���һ���ַ�����
        'Ϊʲô��ô�����أ���ʵ����Ϊ��ʹ y ����ռ��һ���Ŀռ䣬Ҳ���� VB6�г��õ� Dim y As String*256
        '������DLL����ִ����ϣ��᷵��һЩ���ݣ�д��y�����У�256��y�ĳ��ȣ����ص��ַ������Ȳ��ᳬ��256�ֽ�,����NULL��β��һ���ֽ�
        y = "------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------"
        DeviceQuaryCommand(RichTextBox1.Text, y, 256)
        '�������ݣ�д�ص��Ի���
        RichTextBox2.Text = y
    End Sub
End Class
