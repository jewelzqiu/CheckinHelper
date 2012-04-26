using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;



///以下是DLL使用说明
//DLL函数只有一个，输入字符串，输出也是字符串，第三个参数是输出字符串缓冲区的长度



namespace C_SHAP_VS2005_DLL_Relay
{
    public partial class Form1 : Form
    {
        [DllImport("RelayControlCtl.DLL")]

        //修饰符 extern 返回变量类型 方法名称 （参数列表）
        static extern int DeviceQuaryCommand(Byte[] instr, Byte[] outstr, int outstr_len);

        public Form1()
        {
            InitializeComponent();
            richTextBox1.Text = "set_relay_val ETH:TCP:192.168.1.250:2000 R1:ON R2:ON";
        }

        private void buttonSend_Click(object sender, EventArgs e)
        {

            byte[] inbyte = System.Text.Encoding.Default.GetBytes(richTextBox1.Text);
            Byte[] outbyte = new Byte[1024];
            DeviceQuaryCommand(inbyte, outbyte, 1024);
            string str = System.Text.Encoding.Default.GetString(outbyte);
            richTextBox2.Text = str;
        }
    }
}