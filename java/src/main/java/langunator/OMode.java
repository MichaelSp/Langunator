package langunator;

import static langunator.Mode.NONE;

import java.sql.SQLException;
import java.sql.Statement;

public class OMode {

	static private Mode mode = NONE;
	static private short[] modeStat = new short[4];
	static private boolean revMode = false;
	static private byte boxmlt;

	static public Statement stmt;

	private OMode() {
	}

	static void switchRev() {
		OMode.revMode = !(OMode.revMode);
	}

	static boolean getRev() {
		return OMode.revMode;
	}

	static void setMode(Mode m) {
		mode = m;
		for (int n = 0; n < 4; n++)
			modeStat[n] = 0;

	}

	static Mode getMode() {
		return mode;
	}

	static void setBoxmlt(byte b) {
		boxmlt = b;
	}

	static byte getBoxmlt() {
		return boxmlt;
	}

	static String getAskSpecification() {
		switch (mode) {
		case AGAIN:
			return " WHERE " + boxmlt + "^(box)=timeleft;";
		case BOX:
			return " WHERE (timeleft < 1);";
		case NULL:
			return " WHERE (box=0);";
		case ALL:
			return ";";
		default:
			return "";
		}
	}

	static String getModeName() {
		switch (mode) {
		case NULL:
			return "0";
		default:
			return mode.toString();
		}
	}

	static boolean isAbfrage() {
		switch (mode) {
		case NONE:
		case LOADED: {
			return false;
		}
		default:
			return true;
		}
	}

	static void changings(int key, int oldBox, int oldTime, boolean bDay, String id, String pool) throws SQLException {
		int newBox = 0;
		switch (key) {
		case 37:
			newBox = oldBox - 1;
			break; // begin NEW-Box-Bestimmung
		case 38:
			newBox = oldBox;
			break;
		case 39:
			newBox = oldBox + 1;
			break;
		case 40:
			newBox = 0;
			break;
		}
		if ((key > 47) && (key < 58))
			newBox = key - 48;
		if (newBox < 0)
			newBox = 0; // end NEW-Box-Bestimmung

		final int iExp = (int) Math.pow(boxmlt, newBox);
		final int newTime = bDay ? (oldTime + iExp) : iExp;
		try {
			stmt.executeUpdate("UPDATE " + pool + " SET box=" + newBox + ", timeleft=" + newTime + " WHERE lang1=$aa$" + id
					+ "$aa$;");

		} catch (final Exception ex) {
			System.out.println("Ex. OMode - changings: " + ex.getMessage());
		}
	}

	static void updateModeStat(int key) {
		if ((key > 36) && (key < 41))
			modeStat[key - 37]++;
	}

	static short[] getModeStat() {
		return modeStat;
	}

}