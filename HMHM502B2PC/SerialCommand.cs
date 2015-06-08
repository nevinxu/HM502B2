namespace MotionSensor
{
    public partial class SerialCommand
    {
        private void PairingCommand()
        {
            if (DebugMode == 1)
            {
                ;
            }
            else if (DebugMode == 2)
            {
                byte[] ssss = { 0x77, 0x20, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

                string InputChar = textBox5.Text;
                char[] cc = InputChar.ToCharArray();
                for (int i = 0; i < 0x11; i++)
                {
                    if (cc[i] >= '0' && (cc[i] <= '9'))
                    {
                        cc[i] = (char)(cc[i] - 0x30);
                    }
                    else if (cc[i] >= 'A' && (cc[i] <= 'E'))
                    {
                        cc[i] = (char)(cc[i] - 'A' + 10);
                    }
                }
                for (int i = 0; i < 6; i++)
                {
                    ssss[4 + i] = (byte)((cc[(5 - i) * 3] << 4) + (cc[(5 - i) * 3 + 1]));
                }


                //   ECGPairMAC
                SerialPort.Write(ssss, 0, 10);
                System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                string DisplayString = "请求设置配对心电补丁的MAC\r\n";
                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                OutMsg(MonitorText, DisplayString, Color.Red);
            }

        }
    }


}