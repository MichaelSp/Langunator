package langunator;

import static langunator.OMode.stmt;

import java.awt.GridBagConstraints;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.sql.Connection;
import java.sql.DriverManager;

import javax.swing.JButton;
import javax.swing.WindowConstants;

import ownComponents.JFrameGridBag;
import ownComponents.JTextAreaX;

public class Introduction extends JFrameGridBag {

	private static final long serialVersionUID = -7975453214938325039L;
	JTextAreaX ta = new JTextAreaX();
	JButton bOK = new JButton("Initialisieren");
	JButton bAbort = new JButton("Abbrechen");

	GridBagConstraints gbc = new GridBagConstraints();

	Introduction(boolean firstTime, final int iCompletenessIndex) {
		super("Einleitung");
		setDefaultCloseOperation(WindowConstants.DISPOSE_ON_CLOSE);
		addComp(ta, 0, 0, 2, 1, 1.0, 1.0);
		if (firstTime) {
			addComp(bOK, 0, 1, 1, 1, 0.5, 0);
			addComp(bAbort, 1, 1, 1, 1, 0.5, 0);
		}
		setBounds(300, 100, 400, 400);
		setVisible(true);

		if (firstTime) {
			bOK.addActionListener(new ActionListener() {
				private final boolean isPostgre = false;

				public void actionPerformed(ActionEvent ae) {
					try {
						Class.forName("org.sqlite.JDBC");
						final Connection cn = DriverManager.getConnection("jdbc:sqlite:db.sqlite");
						stmt = cn.createStatement();

						if (iCompletenessIndex < 1 && isPostgre) {
							stmt.executeUpdate("CREATE DATABASE langunator WITH OWNER = postgres ENCODING = 'UTF8';");
						}
						if (iCompletenessIndex < 2) {
							stmt.executeUpdate("CREATE TABLE pools( lang1 text, lang2 text, font1 text, "
									+ "font2 text, previous int2, size1 int2, size2 int2, boxmlt int2, "
									+ "id text NOT NULL, CONSTRAINT pools_pkey PRIMARY KEY (id)) ");
						}
						try {
							stmt.executeUpdate("DROP TABLE stuff; ");
						} catch (final Exception e) {
						}
						stmt.executeUpdate("CREATE TABLE stuff(autoload boolean, viewinfo boolean, "
								+ "viewnotes boolean, viewstatus boolean, daybyday boolean, notestext text, "
								+ "primkey boolean NOT NULL, editorder integer, "
								+ "CONSTRAINT stuff_pkey PRIMARY KEY (primkey) )");
						stmt.executeUpdate("INSERT INTO stuff VALUES (0, 1, 0, 0, 0, '', 1, 0);");
						new MainFrame();
						killFrame();
					} catch (final ClassNotFoundException e) {
						ta.setText("Fehler!");
					}

					catch (final Exception e) {
						ta.setText("Fehler!");
						ta.addLine("Exception-Message: " + e.getMessage());
						ta.addLine("");
						ta.addLine("Langunator konnte nicht intialisiert werden.");
						ta.addLine("Stellen Sie sicher, dass Sie alle Anweisungen in der Langunator-Anleitung befolgt haben");
						ta.addLine("");
						ta.addLine("Was nat�rlich immer helfen kann, ist, den PC neu zu starten :-)");
					}
				}
			});
			bAbort.addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent ae) {
					killFrame();
				}
			});
		} // END if (first time)
		if (iCompletenessIndex < 2) {
			ta.addLine("Initialisierung");
			ta.addLine("");
			ta.addLine("Klicken Sie unten auf 'Initialisieren', um in PostgreSQL das Grundgerüst zu schaffen, ");
			ta.addLine("mit dem Ihre Datenbank bereit wird, Vokabeln zu speichern und zu verwalten. ");
			ta.addLine("");
			ta.addLine("");
		} else {
			ta.addLine(" Updater");
			ta.addLine("");
			ta.addLine("Zu ihrer aktuellen Langunator-Version müssen die ");
			ta.addLine("Daten in PostgreSQL angepasst werden");
			ta.addLine("");
			ta.addLine("Klicken Sie auf 'Intitialisieren', um dies zu bewerkstelligen");
		}
	}

	void killFrame() {
		this.dispose();
	}

}
