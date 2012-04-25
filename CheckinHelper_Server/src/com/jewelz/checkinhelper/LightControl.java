package com.jewelz.checkinhelper;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;

public class LightControl {

	static String IP = "";
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
		
		return false;
	}
		
	public boolean lightOff() {
	
		return false;
	}
	
}
