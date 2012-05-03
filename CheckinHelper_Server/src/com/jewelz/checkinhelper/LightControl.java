package com.jewelz.checkinhelper;

import java.io.BufferedOutputStream;
import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.net.Socket;
import java.net.UnknownHostException;

public class LightControl {

	static String IP = "192.168.0.250";
	static final int port = 2000;
	static String FILENAME = "lightip";

	public LightControl() {
		try {
			BufferedReader reader = new BufferedReader(new FileReader(FILENAME));
			IP = reader.readLine();
		} catch (FileNotFoundException e) {
			File file = new File(FILENAME);
			try {
				file.createNewFile();
			} catch (IOException e1) {
				e1.printStackTrace();
			}
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	public boolean lightOn() {
		try {
			Socket socket = new Socket(IP, port);
			DataOutputStream out = new DataOutputStream(
					new BufferedOutputStream(socket.getOutputStream()));
			byte[] cmd = new byte[12];
			cmd[0] = 0x3;
			cmd[1] = 0x25;
			cmd[2] = 0x0;
			cmd[3] = 0x0;
			cmd[4] = 0x5;
			cmd[5] = 0x0;
			cmd[6] = 0x0;
			cmd[7] = 0x4;
			cmd[8] = 0x0C; // output control
			cmd[9] = 0x0;
			cmd[10] = 0x0;
			cmd[11] = 0x0;
			out.write(cmd);
			out.close();
			socket.close();
			return true;
		} catch (UnknownHostException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
		return false;
	}

	public boolean lightOff() {
		try {
			Socket socket = new Socket(IP, port);
			DataOutputStream out = new DataOutputStream(
					new BufferedOutputStream(socket.getOutputStream()));
			byte[] cmd = new byte[12];
			cmd[0] = 0x3;
			cmd[1] = 0x25;
			cmd[2] = 0x0;
			cmd[3] = 0x0;
			cmd[4] = 0x5;
			cmd[5] = 0x0;
			cmd[6] = 0x0;
			cmd[7] = 0x4;
			cmd[8] = 0x0; // output control
			cmd[9] = 0x0;
			cmd[10] = 0x0;
			cmd[11] = 0x0;
			out.write(cmd);
			out.close();
			socket.close();
			return true;
		} catch (UnknownHostException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
		return false;
	}
	
}
