package langunator;

import java.awt.Font;

public class util {

	static long getDay() {
		final java.util.Date start = new java.util.Date();
		return start.getTime() / (60000 * 60 * 24);
	}

	static Font FontX(int size) {
		return new Font("Arial", Font.PLAIN, size);
	}

}
