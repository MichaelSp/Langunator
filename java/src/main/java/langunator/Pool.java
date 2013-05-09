package langunator;

import static langunator.Mode.LOADED;
import static langunator.OMode.stmt;

import java.awt.Font;
import java.sql.ResultSet;
import java.sql.SQLException;

public class Pool {

	private String lang1, lang2;
	private String ID;
	private String message;
	private Font fLang1, fLang2;
	private byte fontSize1, fontSize2;
	private String fontName1, fontName2;
	private byte boxMlt;

	Pool() {
	}

	Pool(String name) {
		loadPool(name);
	}

	String getID() {
		return ID;
	}

	String getName() {
		return lang1 + "-" + lang2 + " pool";
	}

	String getMessage() {
		return message;
	}

	String getLang1() {
		return lang1;
	}

	String getLang2() {
		return lang2;
	}

	Font getFont1() {
		return fLang1;
	}

	Font getFont2() {
		return fLang2;
	}

	String getFontName1() {
		return fontName1;
	}

	String getFontName2() {
		return fontName2;
	}

	byte getFontSize1() {
		return fontSize1;
	}

	byte getFontSize2() {
		return fontSize2;
	}

	byte getBoxMlt() {
		return boxMlt;
	}

	boolean loadPool(String poolName) {
		int exceptionEnum = 0;
		try {
			final ResultSet rs = stmt.executeQuery("SELECT * FROM pools WHERE (id='" + poolName + "');");
			rs.next();
			ID = poolName;
			exceptionEnum++;
			lang1 = rs.getString("lang1");
			lang2 = rs.getString("lang2");
			fontName1 = rs.getString("font1");
			fontName2 = rs.getString("font2");
			fontSize1 = Byte.parseByte(rs.getString("size1"));
			fontSize2 = Byte.parseByte(rs.getString("size2"));
			fLang1 = new Font(fontName1, Font.PLAIN, fontSize1);
			fLang2 = new Font(fontName2, Font.PLAIN, fontSize2);
			final int timeDifference = (int) util.getDay() - rs.getInt("previous");
			boxMlt = (byte) rs.getInt("boxmlt");
			exceptionEnum++;
			OMode.setBoxmlt((byte) rs.getInt("boxmlt"));
			exceptionEnum++;
			stmt.executeUpdate("UPDATE pools SET previous = previous + " + timeDifference + " WHERE " + "(id='" + ID + "');");
			stmt.executeUpdate("UPDATE " + ID + " SET timeleft = timeleft - " + timeDifference + ";");
			OMode.setMode(LOADED);

			message = "Passed days since last initiation: " + timeDifference;

			return true;
		} catch (final SQLException ex) {
			switch (exceptionEnum) {
			case 0:
				message = "SQL-Exc.: There is no " + lang1 + "-" + lang2 + " pool";
				break;
			case 1:
				message = "Error caused by Fonts of " + lang1 + "-" + lang2 + " pool";
				break;
			case 2:
				message = "Error caused by 'Box Time Multiplier'";
				break;
			case 3:
				message = "Error on updating times. Bad for BOX-Mode.";
				break;
			}
			message = message + "  ..... " + ex.getMessage();
		}
		return false;
	}

}
