using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO.Ports;

namespace RFtest
{
    class SerialCommand
    {
        public void Upgrade_PingPacketCommand(SerialPort serialport)
        {
            byte[] ssss = { 0x5a, 0xa6 };
            if (serialport.IsOpen)
            {
                serialport.Write(ssss, 0, 2);
            }
        }
        public void Upgrade_ACKCommand(SerialPort serialport)
        {
            byte[] ssss = { 0x5a, 0xa1 };
            if (serialport.IsOpen)
            {
                serialport.Write(ssss, 0, 2);
            }
        }

        public void FlashEraseAll_Command(SerialPort serialport)
        {
            byte[] ssss = { 0x5a, 0xa4, 0x04, 0x00, 0xc4, 0x2e, 0x01, 0x00, 0x00, 0x00 };
            if (serialport.IsOpen)
            {
                serialport.Write(ssss, 0, 10);
            }
        }

        public void FlashEraseAllUnsecure_Command(SerialPort serialport)
        {
            byte[] ssss = { 0x5a, 0xa4, 0x04, 0x00, 0xcf, 0x32, 0x0d, 0x00, 0xcc, 0x00 };
            if (serialport.IsOpen)
            {
                serialport.Write(ssss, 0, 10);
            }
        }


        public UInt16 crc16_update(byte[] Byte, UInt32 lengthInBytes)
        {
            UInt32 crc = 0;
            UInt32 j;
            for (j = 0; j < lengthInBytes; ++j)
            {
                UInt32 i;
                UInt32 bytes = (UInt32)Byte[j];
                crc ^= bytes << 8;
                for (i = 0; i < 8; ++i)
                {
                    UInt32 temp = crc << 1;
                    if ((crc & 0x8000) == 0x8000)
                    {
                        temp ^= 0x1021;
                    }
                    crc = temp;
                }
            }
            return (UInt16)crc;
        }

        public void WriteMemory_Command(SerialPort serialport, Int32 StartAddress, Int32 ByteCount)
        {
            byte[] sss = { 0x5a, 0xa4, 0x0c, 0x00, 0x56, 0x2b, 0x04, 0x00, 0x00, 0x02, 0x00, 0x04, 0x00, 0x20, 0x64, 0x00, 0x00, 0x00 };

            sss[10] = (byte)(StartAddress);
            sss[11] = (byte)(StartAddress >> 8);
            sss[12] = (byte)(StartAddress >> 16);
            sss[13] = (byte)(StartAddress >> 24);

            sss[14] = (byte)(ByteCount);
            sss[15] = (byte)(ByteCount >> 8);
            sss[16] = (byte)(ByteCount >> 16);
            sss[17] = (byte)(ByteCount >> 24);

            byte[] ssss = new byte[16];
            for (int i = 0; i < 4; i++)
            {
                ssss[i] = sss[i];
            }
            for (int i = 4; i < 16; i++)
            {
                ssss[i] = sss[i + 2];
            }

            UInt16 crc_buffer = crc16_update(ssss, 16);

            sss[4] = (byte)(crc_buffer);
            sss[5] = (byte)(crc_buffer >> 8);

            if (serialport.IsOpen)
            {
                serialport.Write(sss, 0, 18);
            }
        }
        public void WriteMemoryData_Command(SerialPort serialport, byte[] Byte, UInt16 Length)
        {
            byte[] ssss = new byte[100];
            byte[] sss = new byte[100];

            ssss[0] = 0x5A;
            ssss[1] = 0xA5;
            ssss[2] = (byte)Length;
            ssss[3] = (byte)(Length >> 8);

            for (int i = 0; i < Length; i++)
            {
                ssss[4 + i] = Byte[i];
            }
            UInt16 crc_buffer = crc16_update(ssss, (UInt32)4 + Length);

            for (int i = 0; i < 4; i++)
            {
                sss[i] = ssss[i];
            }
            sss[4] = (byte)crc_buffer;
            sss[5] = (byte)(crc_buffer >> 8);
            for (int i = 0; i < Length; i++)
            {
                sss[6 + i] = ssss[4 + i];
            }

            if (serialport.IsOpen)
            {
                serialport.Write(sss, 0, 6 + Length);
            }
        }

        public void ReadMemory_Command(SerialPort serialport, Int32 StartAddress, Int32 ByteCount)
        {
            byte[] sss = { 0x5a, 0xa4, 0x0c, 0x00, 0x56, 0x2b, 0x03, 0x00, 0x00, 0x02, 0x00, 0x04, 0x00, 0x20, 0x64, 0x00, 0x00, 0x00 };

            sss[10] = (byte)(StartAddress);
            sss[11] = (byte)(StartAddress >> 8);
            sss[12] = (byte)(StartAddress >> 16);
            sss[13] = (byte)(StartAddress >> 24);

            sss[14] = (byte)(ByteCount);
            sss[15] = (byte)(ByteCount >> 8);
            sss[16] = (byte)(ByteCount >> 16);
            sss[17] = (byte)(ByteCount >> 24);

            byte[] ssss = new byte[16];
            for (int i = 0; i < 4; i++)
            {
                ssss[i] = sss[i];
            }
            for (int i = 4; i < 16; i++)
            {
                ssss[i] = sss[i + 2];
            }

            UInt16 crc_buffer = crc16_update(ssss, 16);

            sss[4] = (byte)(crc_buffer);
            sss[5] = (byte)(crc_buffer >> 8);

            if (serialport.IsOpen)
            {
                serialport.Write(sss, 0, 18);
            }
        }

        public void SendConnectSerialCommand(SerialPort serialport)
        {
                byte[] ssss = { 0x77, 0x01, 0x00, 0x00 };
                serialport.Write(ssss, 0, 4);
        }
        public void SendDisConnectSerialCommand(SerialPort serialport)
        {
            byte[] ssss = new byte[4];
            ssss[0] = 0x77;
            ssss[1] = 0x0C;
            ssss[2] = 0x00;
            ssss[3] = 0x00;
            serialport.Write(ssss, 0, 4);
        }
        public void SendDisAdvertiseSerialCommand(SerialPort serialport)
        {
                byte[] ssss = { 0x77, 0x03, 0x00, 0x00 };
                serialport.Write(ssss, 0, 4);
                
        }
        public void ConnectBLESerialCommand(SerialPort serialport, byte SelectedIndex)
        {
                if (serialport.IsOpen)
                {
                    byte[] ssss = { 0x77, 0x05, 0x00, 0x01, 0x00 };
                    ssss[4] = SelectedIndex;
                    if (serialport.IsOpen)
                    {
                            serialport.Write(ssss, 0, 5);
                    }
                }
        }
        public void DisconnectBLESerialCommand(SerialPort serialport)
        {
            if (!serialport.IsOpen)   //检测串口是否关闭
                {
                    return;
                }
                byte[] ssss = { 0x77, 0x0A, 0x00, 0x00 };
                serialport.Write(ssss, 0, 4);

        }
        public void ReceiveECGDataSerialCommand(SerialPort serialport)
        {
                byte[] ssss = { 0x77, 0x14, 0x00, 0x00 };
                if (serialport.IsOpen)
                {
                    serialport.Write(ssss, 0, 4);
                }


        }
        public void StopReceiveECGDataSerialCommand(SerialPort serialport)
        {
                byte[] ssss = { 0x77, 0x16, 0x00, 0x00 };
                if (serialport.IsOpen)
                {
                    serialport.Write(ssss, 0, 4);
                }
                

        }
        public void ReceiveECGPatchIDSerialCommand(SerialPort serialport)
        {
                byte[] ssss = { 0x77, 0x18, 0x00, 0x00 };
                if (serialport.IsOpen)
                {
                    serialport.Write(ssss, 0, 4);
                }
                

        }
        public void ReceiveCentralMACCommand(SerialPort serialport)
        {
                byte[] ssss = { 0x77, 0x1C, 0x00, 0x00 };
                if (serialport.IsOpen)
                {
                    serialport.Write(ssss, 0, 4);
                }
                

        }
        public void ReceiveECGPatchMACCommand(SerialPort serialport)
        {
                byte[] ssss = { 0x77, 0x1E, 0x00, 0x00 };
                if (serialport.IsOpen)
                {
                    serialport.Write(ssss, 0, 4);
                }
                

        }
        public void ReceivePairingStatusCCommand(SerialPort serialport)
        {
                byte[] ssss = { 0x77, 0x22, 0x00, 0x00 };
                //   ECGPairMAC
                if (serialport.IsOpen)
                {
                    serialport.Write(ssss, 0, 4);
                }

        }
        public void SetTestModeCommand(SerialPort serialport)
        {
                byte[] ssss = { 0x77, 0x24, 0x00, 0x01, 0x01 };
                if (serialport.IsOpen)
                {
                    serialport.Write(ssss, 0, 5);
                }


        }
        public void AutoConnectBLESerialCommand(SerialPort serialport)
        {
                byte[] ssss = { 0x77, 0x11, 0x00, 0x01, 0x01 };
                if (serialport.IsOpen)
                {
                    serialport.Write(ssss, 0, 5);
                }
                

        }
        public void DisAutoConnectBLESerialCommand(SerialPort serialport)
        {
                byte[] ssss = { 0x77, 0x11, 0x00, 0x01, 0x00 };
                    if (serialport.IsOpen)
                    {
                        serialport.Write(ssss, 0, 5);
                    }
                    

        }
        public void AutoConnectBLEStatusSerialCommand(SerialPort serialport)
        {
                byte[] ssss = { 0x77, 0x1A, 0x00, 0x00 };
                if (serialport.IsOpen)
                {
                    serialport.Write(ssss, 0, 4);
                }
                
        }
        public void PairingCommand(SerialPort serialport, byte[] PairMAC)
        {
                byte[] ssss = { 0x77, 0x20, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
                for (int i = 0; i < 6; i++)
                {
                    ssss[4 + i] = PairMAC[i];
                }
                //   ECGPairMAC
                if (serialport.IsOpen)
                {
                    serialport.Write(ssss, 0, 10);
                }
                

        }
        public void calibration0mvSerialCommand(SerialPort serialport)
        {
                byte[] ssss = { 0x77, 0x2A, 0x00, 0x00 };
                if (serialport.IsOpen)
                {
                    serialport.Write(ssss, 0, 4);
                }
                
        }
        public void calibration1mvSerialCommand(SerialPort serialport)
        {
                byte[] ssss = { 0x77, 0x26, 0x00, 0x00 };
                if (serialport.IsOpen)
                {
                    serialport.Write(ssss, 0, 4);
                }
                
        }

        public void Setcalibration0mvSerialCommand(SerialPort serialport, int difference_Value)
        {
                byte[] ssss = { 0x77, 0x2C, 0x00, 0x02, 0x00, 0x00 };
                int m_buffer = difference_Value % 256;
                ssss[4] = (byte)m_buffer;
                m_buffer = difference_Value / 256;
                ssss[5] = (byte)m_buffer;
                if (serialport.IsOpen)
                {
                    serialport.Write(ssss, 0, 6);
                }

                
        }
        public void Setcalibration1mvSerialCommand(SerialPort serialport, int amplification)
        {
                byte[] ssss = { 0x77, 0x28, 0x00, 0x02, 0x00, 0x00 };
                int m_buffer = amplification % 256;
                ssss[4] = (byte)m_buffer;
                m_buffer = amplification / 256;
                ssss[5] = (byte)m_buffer;
                if (serialport.IsOpen)
                {
                    serialport.Write(ssss, 0, 6);
                }

                
        }
        public void GetHardWareVersionSerialCommand(SerialPort serialport)
        {
                byte[] ssss = { 0x77, 0x30, 0x00, 0x00 };
                if (serialport.IsOpen)
                {
                    serialport.Write(ssss, 0, 4);
                }
                
        }
        public void GetSoftWareVersionSerialCommand(SerialPort serialport)
        {
                byte[] ssss = { 0x77, 0x32, 0x00, 0x00 };
                if (serialport.IsOpen)
                {
                    serialport.Write(ssss, 0, 4);
                }
                
        }
        public void UpGradeCommand(SerialPort serialport)
        {
            byte[] ssss = new byte[2];
            ssss[0] = 0x5A;
            ssss[1] = 0xA6;
            serialport.Write(ssss, 0, 2);
        }

    }
}
