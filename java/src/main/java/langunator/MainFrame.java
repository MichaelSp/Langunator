package langunator;

import static langunator.Mode.AGAIN;
import static langunator.Mode.ALL;
import static langunator.Mode.BOX;
import static langunator.Mode.LOADED;
import static langunator.Mode.NONE;
import static langunator.Mode.NULL;
import static langunator.OMode.changings;
import static langunator.OMode.getAskSpecification;
import static langunator.OMode.getMode;
import static langunator.OMode.getModeName;
import static langunator.OMode.getModeStat;
import static langunator.OMode.isAbfrage;
import static langunator.OMode.setMode;
import static langunator.OMode.stmt;
import static langunator.OMode.updateModeStat;

import java.awt.Color;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.Event;
import java.awt.GridBagConstraints;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.InputEvent;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

import javax.swing.JCheckBoxMenuItem;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.JToolBar;

import ownComponents.JFrameGridBag;
import ownComponents.JQuickCMI;
import ownComponents.JQuickMI;
import ownComponents.JTextAreaX;

public class MainFrame extends JFrameGridBag {

	private static final long serialVersionUID = 1557725435555837610L;
	JTextField tfAsk = new JTextField();
	JTextAreaX taAns = new JTextAreaX();
	JScrollPane spAns = new JScrollPane(taAns);
	JTextArea taNotes = new JTextArea();
	JScrollPane spNotes = new JScrollPane(taNotes);
	JTextAreaX taInfo = new JTextAreaX();
	JScrollPane spInfo = new JScrollPane(taInfo);
	JToolBar tb = new JToolBar();

	JLabel lStatAllName = new JLabel("ALL:");
	JLabel lStatAll = new JLabel("");
	int iStatAll;
	JLabel[] lStatusName = new JLabel[12];
	JLabel[] lStatus = new JLabel[12];
	Container[] cStat = new Container[12];
	int[] iStatus = new int[12];

	JMenuBar menuBar = new JMenuBar();
	JMenu menu, subMenu;
	JMenuItem menuLoadAll;
	JCheckBoxMenuItem mICNotes, mICInfo, mICStatus, mICAutoLoad, mICDayByDay;

	GridBagConstraints gbc = new GridBagConstraints();

	String title;
	String sAutoLoadPoolName = "";

	boolean asked, allToLoad = false;

	int timeDifference;
	static int iEditOrder;

	ResultSet pools, abfragePool;
	Statement stmtPools, stmtNowPool;

	Pool nowPool;

	// STINKNORMALER KONSTRUKTOR
	public MainFrame() {
		super("Langunator");
		setLocation(250, 200);
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

		addCompontents();
		createToolBar();

		int iCompletenessIndex = 0;
		try {
			dbConnect();
			iCompletenessIndex = createMenu(iCompletenessIndex);
		} catch (final ClassNotFoundException ex) {
			taAns.setText("! Driver-Error ! Could not connect to Database");
			taAns.addLine("This may have different reasons: ");
			taAns.addLine("1) Postgre-SQL is not installed on your computer");
			taAns.addLine("2) You do not have the JavaDevelopmentKit (JDK) on your computer");
			taAns.addLine("");
		} catch (final SQLException ex) {
			ex.printStackTrace();
			new Introduction(true, iCompletenessIndex);
		}

		updateView();

		setVisible(true);

	} // END of Constructor

	private void addCompontents() {
		addComp(tfAsk, 0, 0, 1, 1, 0.5, 0);
		addComp(spAns, 0, 1, 1, 1, 0.5, 0.4);
		spAns.setPreferredSize(new Dimension(600, 250));
		addComp(spInfo, 0, 2, 1, 1, 0.5, 0.4);
		spInfo.setPreferredSize(new Dimension(600, 150));
		addComp(spNotes, 0, 3, 1, 1, 0.5, 0.4);
		spNotes.setPreferredSize(new Dimension(600, 100));
		addComp(tb, 0, 4, 1, 1, 0.5, 0);
		taInfo.setText("~ INFO ~");
		taInfo.setToolTipText("INFO-TEXT!");
	}

	private void createToolBar() {
		tb.add(lStatAllName);
		tb.add(lStatAll);
		for (int n = 0; n < 12; n++) {
			cStat[n] = new Container();
			// cStat[n].setVisible(true);
			tb.add(cStat[n]);
			lStatusName[n] = new JLabel("  " + n + ": ");
			lStatusName[n].setForeground(new Color(0, 50, 255));
			tb.add(lStatusName[n]);
			lStatus[n] = new JLabel("");
			tb.add(lStatus[n]);
		}
	}

	private int createMenu(int iCompletenessIndex) throws ClassNotFoundException, SQLException {
		ResultSet rsVar;
		boolean bviewNotes;
		boolean bviewInfo;
		boolean bviewStatus;
		boolean bAutoLoad;
		boolean bDayByDay;

		menu = new JMenu("Pool");
		menuLoadAll = new JQuickMI("Neu", KeyEvent.VK_F1, Event.CTRL_MASK);
		menuLoadAll.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent a) { // MENU - POOL - NEW
				castNewFrame();
			}
		});
		menu.add(menuLoadAll);

		// Globale ResultSets brauchen einzelne Statements, um nicht
		// geschlossen zu werden.

		iCompletenessIndex++;

		pools = stmtPools.executeQuery("SELECT * FROM pools;");
		iCompletenessIndex++;

		rsVar = stmt.executeQuery("SELECT * FROM stuff;");
		rsVar.next();
		bviewNotes = rsVar.getBoolean("viewnotes");
		bviewInfo = rsVar.getBoolean("viewinfo");
		bviewStatus = rsVar.getBoolean("viewstatus");
		bAutoLoad = rsVar.getBoolean("autoload");
		bDayByDay = rsVar.getBoolean("daybyday");
		taNotes.setText(rsVar.getString("notestext"));
		iEditOrder = rsVar.getInt("editorder");
		tfAsk.addKeyListener(new CursorPressedListener()); // TF get Eigenschaft
															// auf Keys zu
															// reagieren
		taAns.addKeyListener(new CursorPressedListener()); // TA und

		createSubmenuLoad();
		menu.add(subMenu);

		menuLoadAll = new JQuickMI("Alle Laden", KeyEvent.VK_F3, Event.CTRL_MASK);
		menuLoadAll.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				try {
					allToLoad = true;
					pools.beforeFirst(); // MENU - POOL - LOAD ALL
					nextPool();
				} catch (final SQLException ex) {
					taInfo.addLine("SQL-Ex. @ Load All: " + ex.getMessage());
				}
			}
		});
		menu.add(menuLoadAll);

		menu.addSeparator();

		menuLoadAll = new JQuickMI("Vokabeln verwalten", KeyEvent.VK_F4, Event.CTRL_MASK);
		menuLoadAll.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) { // MENU - POOL -
															// EDIT
				new EditFrame(nowPool, tfAsk.getText(), iEditOrder);
			}
		});
		menu.add(menuLoadAll);

		menuLoadAll = new JQuickMI("Poolformat verwalten", KeyEvent.VK_F5, Event.CTRL_MASK);
		menuLoadAll.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) { // MENU - POOL -
															// EDIT
				castChangeFrame();
			}
		});
		menu.add(menuLoadAll);

		menu.addSeparator();

		menuLoadAll = new JQuickMI("Löschen", KeyEvent.VK_F6, Event.CTRL_MASK);
		menuLoadAll.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) { // MENU - POOL -
															// EDIT
				castDeleteFrame();
			}
		});
		menu.add(menuLoadAll);

		menu.addSeparator();
		menu.addSeparator();

		menuLoadAll = new JQuickMI("Statistik", "F7");
		menuLoadAll.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) { // MENU - POOL -
															// STATISTICS
				try {
					new StatFrame(nowPool.getID());
				} catch (final NullPointerException ex) {
					new StatFrame();
				}
			}
		});
		menu.add(menuLoadAll);
		menuBar.add(menu);

		menu = new JMenu("Abfrage");
		menuLoadAll = new JQuickMI("Box-Modus", "F1");
		menuLoadAll.addActionListener(new ModeListener(BOX));

		menu.add(menuLoadAll);
		menuLoadAll = new JQuickMI("0-Modus", "F2");
		menuLoadAll.addActionListener(new ModeListener(NULL));
		menu.add(menuLoadAll);

		menuLoadAll = new JQuickMI("Wiederholungs-Modus", "F3");
		menuLoadAll.addActionListener(new ModeListener(AGAIN));
		menu.add(menuLoadAll);

		menuLoadAll = new JQuickMI("ALLES!", "F4");
		menuLoadAll.addActionListener(new ModeListener(ALL));
		menu.add(menuLoadAll);

		menu.addSeparator();

		menuLoadAll = new JQuickCMI("Sprachen tauschen", "F5");
		menuLoadAll.addActionListener(new RevListener());
		menu.add(menuLoadAll);

		mICDayByDay = new JCheckBoxMenuItem("Tag für Tag (Box-Modus)");
		mICDayByDay.setState(bDayByDay);
		menu.add(mICDayByDay);

		menu.addSeparator();

		menuLoadAll = new JQuickMI("Zurück", "F10");
		menuLoadAll.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent ae) {
				taInfo.addLine("Sorry, not working yet!");
			}
		});
		menu.add(menuLoadAll);

		menuBar.add(menu);

		menu = new JMenu("Ansicht");

		mICNotes = new JQuickCMI("Notizen", KeyEvent.VK_N, InputEvent.ALT_DOWN_MASK);
		mICNotes.addActionListener(new ViewListener());
		mICNotes.setState(bviewNotes);
		menu.add(mICNotes);

		mICInfo = new JQuickCMI("Info", KeyEvent.VK_I, InputEvent.ALT_DOWN_MASK);
		mICInfo.addActionListener(new ViewListener());
		mICInfo.setState(bviewInfo);
		menu.add(mICInfo);

		mICStatus = new JQuickCMI("Status-Leiste", KeyEvent.VK_S, InputEvent.ALT_DOWN_MASK);
		mICStatus.addActionListener(new ViewListener());
		mICStatus.setState(bviewStatus);
		menu.add(mICStatus);

		menuBar.add(menu);

		menu = new JMenu("Extras");
		menuLoadAll = new JMenuItem("Backup (SQL-Text)");
		menuLoadAll.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent ex) { // IMPROVE !!!
				taInfo.setText("");
				try {
					final ResultSet rsID = stmtPools.executeQuery("SELECT (id) FROM pools;");

					while (rsID.next()) {
						final String poolID = rsID.getString("id");
						final ResultSet rsOnePool = stmtNowPool.executeQuery("SELECT * FROM " + poolID + ";");
						taInfo.addLine("INSERT INTO " + poolID + " VALUES");
						boolean first = true;
						String s;
						while (rsOnePool.next()) {
							if (!first) {
								s = ", (";
							} else {
								s = " (";
							}
							first = false;

							for (int c = 1; c < 3; c++) {
								s = s + "$aa$" + rsOnePool.getString(c) + "$aa$, ";
							}
							s = s + rsOnePool.getInt(3) + ", " + rsOnePool.getInt(4) + ")";
							taInfo.addLine(s);

						} // END Loop for Data in one Pool
						taInfo.addLine(";"); // END of SQL-Command - for ONE
												// Pool
						taInfo.addLine(""); // One free Line

					} // END Loop for dif. Pools
				} // END try
				catch (final Exception exc) {
					System.out.println("Exception " + exc.getMessage());
				} // END catch
			} // END actionPerformed
		});
		menu.add(menuLoadAll);

		menuLoadAll = new JMenuItem("Infofeld leeren");
		menuLoadAll.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent ae) {
				taInfo.setText("~ INFO ~");
			}
		});
		menu.add(menuLoadAll);

		mICAutoLoad = new JCheckBoxMenuItem("Sofort-Laden");
		mICAutoLoad.setState(bAutoLoad);
		menu.add(mICAutoLoad);

		menuBar.add(menu);

		menu = new JMenu("Help");
		menuLoadAll = new JMenuItem("Program Info");
		menuLoadAll.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent ae) {
				taInfo.addLine("***");
				taInfo.addLine("***");
				taInfo.addLine("***");
				taInfo.addLine("Version: 3.0 (10.05.13)");
				taInfo.addLine("Author: Peter Wichers");
				taInfo.addLine("");
			}
		});
		menu.add(menuLoadAll);

		menuLoadAll = new JMenuItem("Cursor - Explanation");
		menuBar.add(menu);

		setJMenuBar(menuBar); // Das Menu-Objekt wird jetzt erst in den
								// Frame gesetzt.

		this.addWindowListener(new CloseWindowAction());

		try {
			if (mICAutoLoad.getState()) {
				loadPool(sAutoLoadPoolName);
			}
		} catch (final Exception e) {
			System.out.println("kein Autoload möglich");
		}
		return iCompletenessIndex;
	}

	private void dbConnect() throws ClassNotFoundException, SQLException {
		Class.forName("org.sqlite.JDBC");
		final Connection cn = DriverManager.getConnection("jdbc:sqlite:db.sqlite");
		stmt = cn.createStatement();
		stmtPools = cn.createStatement();
		stmtNowPool = cn.createStatement();
	}

	private void createSubmenuLoad() {
		subMenu = new JMenu("Laden");
		try {
			boolean loaded = false;
			while (pools.next()) {
				final String id = pools.getString("id");
				final String name = pools.getString(1) + "-" + pools.getString(2);
				if (!loaded) {
					sAutoLoadPoolName = id;
					loaded = true;
				}
				menuLoadAll = new JMenuItem(name);
				menuLoadAll.addActionListener(new LoadListener(id));
				subMenu.add(menuLoadAll);
			}
		} catch (final SQLException ex) {
			taInfo.addLine("@ Load:  " + ex.getMessage());
		} catch (final NullPointerException ex) {
			System.out.println("Null-PointEX at Pool-Load-Menu");
		} catch (final Exception e) {
			System.out.println(e.getMessage());
		}
	}

	void loadPool(String name) {
		nowPool = new Pool();

		if (nowPool.loadPool(name)) {
			tfAsk.setFont(nowPool.getFont1());
			taAns.setFont(nowPool.getFont2());

			tfAsk.setText("");
			taAns.setText("");
			pack();

			setTitle("Langunator: " + nowPool.getName());
			setMode(LOADED);
			taInfo.addLine("Pool ~" + nowPool.getName() + "~ geladen");
			tfAsk.requestFocus();
		} else {
			taInfo.addLine("Could not load " + name + ".");
		}
		taInfo.addLine(nowPool.getMessage());
	}

	private void updateView() {
		try {
			spInfo.setVisible(mICInfo.getState());
			spNotes.setVisible(mICNotes.getState());
			tb.setVisible(mICStatus.getState());
		} catch (final Exception e) {
			System.out.println(e.getMessage());
		}
		pack();
	}

	private void nextPool() {
		try {
			if (pools.next()) {
				loadPool(pools.getString(8));
			} else {
				taInfo.addLine("Alle pools abgefragt.");
			}
		} catch (final SQLException ex) {
			System.out.println("SQLException Initiierung von nächstem pool");
		}
	}

	void neueAbfrage() {
		try {
			if (abfragePool.next()) {
				if (!OMode.getRev()) {
					tfAsk.setText(abfragePool.getString(1));
					taAns.setText("");
				} else {
					taAns.setText(abfragePool.getString(2));
					tfAsk.setText("");
				}
				asked = true;
			} else {
				abfragePool = null;
				taInfo.addLine("Abfrage-Beendet.  STATISTIK:");
				final short[] x = getModeStat();
				taInfo.addLine("+1   : " + x[2]);
				taInfo.addLine("=     : " + x[1]);
				taInfo.addLine("-1    : " + x[0]);
				taInfo.addLine("->0 : " + x[3]);
				tfAsk.setText("");
				taAns.setText("");
				setMode(LOADED);
				if (allToLoad) {
					nextPool();
				}
			} // END if abfragepool finished
		} catch (final SQLException ex) {
			taInfo.addLine("SQL-Exception beim Fragen: " + ex.getMessage());
		}
	}

	class LoadListener implements ActionListener {
		private final String id;

		LoadListener(String id) {
			this.id = id;
		}

		public void actionPerformed(ActionEvent ae) {
			loadPool(id);
		}
	}

	class RevListener implements ActionListener {
		public void actionPerformed(ActionEvent ae) {
			OMode.switchRev();
			taInfo.addLine("TAUSCH-MODUS " + (OMode.getRev() ? "AN" : "AUS"));
			setTitle("Langunator: " + nowPool.getName() + (OMode.getRev() ? " -REV-" : ""));
		}
	}

	class ModeListener implements ActionListener {
		Mode mode;

		ModeListener(Mode mode) {
			this.mode = mode;
		}

		public void actionPerformed(ActionEvent e) {
			if (getMode() == NONE) {
				taInfo.addLine("Es ist noch kein Pool geladen");
				taInfo.addLine("Daher keine Abfrage möglich");
			} else {
				try {
					setMode(mode);
					final ResultSet $ = stmtNowPool.executeQuery("SELECT (count(box)) FROM " + nowPool.getID()
							+ getAskSpecification());
					$.next();
					final int iVocAmount = $.getInt(1);
					if (iVocAmount == 0) {
						taInfo.addLine("Kein Vokabeln für " + getModeName() + "-Modus.");
						if (allToLoad) {
							nextPool();
						}
					} else { // if voc. exists
						taInfo.addLine(getModeName() + "-Mods ~ Vokabelmenge: " + iVocAmount + ".");
						iStatAll = iVocAmount;
						lStatAll.setText("" + iStatAll);

						abfragePool = stmtNowPool.executeQuery("SELECT * FROM " + nowPool.getID() + getAskSpecification());
						for (int n = 0; n < 10; n++) {
							iStatus[n] = 0;
						}
						while (abfragePool.next()) {
							iStatus[abfragePool.getInt(3)]++;
						}
						for (int n = 0; n < 12; n++) {
							lStatus[n].setText("" + iStatus[n]);
						}
						abfragePool = stmtNowPool.executeQuery("SELECT * FROM " + nowPool.getID() + getAskSpecification());
						neueAbfrage();
					} // END else <=> if iVocAmout > 1
				} // END try
				catch (final Exception ex) {
					System.out.println("Ex. bei ModeListener: " + ex.getMessage());
				}
			} // ELSE
		} // ACTIONPERFORMED
	} // CLASS MODELISTENER

	/*
	 * Listener für Tastatureingaben wird für tfAsk und taAns verwendet reagiert
	 * nur auf Pfeiltasten, Enter und Zahlen
	 */

	class CursorPressedListener implements KeyListener {
		public void keyPressed(KeyEvent e) {

			final int c = e.getKeyCode();

			if ((((c > 36) && (c < 41)) | (c == 10) | ((c > 47) && (c < 58))) && (isAbfrage())) {
				if (asked) {
					try {
						if (!OMode.getRev()) {
							taAns.setText(abfragePool.getString(2));
						} else {
							tfAsk.setText(abfragePool.getString(1));
						}
						taInfo.addLine(tfAsk.getText() + ": " + abfragePool.getInt(3));
						asked = false;
					} // END try
					catch (final SQLException ex) {
						taInfo.addLine("SQL-Exc. @(asked): " + ex.getMessage());
					}
				} // END if (asked)
				else {
					try {
						final int iBox = abfragePool.getInt(3);
						iStatus[iBox]--;
						lStatus[iBox].setText("" + iStatus[iBox]);
						iStatAll--;
						lStatAll.setText("" + iStatAll);

						if (getMode() == BOX) {
							changings(e.getKeyCode(), abfragePool.getInt(3), abfragePool.getInt(4), mICDayByDay.getState(),
									abfragePool.getString(1), nowPool.getID());
						}
					} // END if (BOX)
					catch (final SQLException ex) {
						taInfo.addLine("SQL-Exception beim Laden von Box und Time " + ex.getMessage());
					}
					updateModeStat(e.getKeyCode());

					neueAbfrage();
				} // END else (asked)
			} // END if (fitting key & Abfrage)
			if (!isAbfrage() & e.getKeyCode() == 10) {
				loadPool(tfAsk.getText());
			}

		} // END keyPressed

		public void keyReleased(KeyEvent e) {
		}

		public void keyTyped(KeyEvent e) {
		}
	}

	class CloseWindowAction extends WindowAdapter {
		@Override
		public void windowClosing(WindowEvent e) {
			try {
				stmt.executeUpdate("update stuff set notestext = '" + taNotes.getText() + "', " + "viewinfo = "
						+ spInfo.isVisible() + ", " + "viewnotes = " + spNotes.isVisible() + ", " + "viewstatus = "
						+ tb.isVisible() + ", " + "autoload = " + mICAutoLoad.getState() + ", " + "daybyday = "
						+ mICDayByDay.getState() + ", " + "editorder = " + iEditOrder + " where (primkey = true);");
			} catch (final SQLException ex) {
				System.out.println(ex.getMessage());
			} catch (final Exception ex) {
				System.out.println("Exception beim SchlieÃŸen: " + ex.getMessage());
			}
		}
	}

	/*
	 * Listener für die Umstellung des Views
	 */

	class ViewListener implements ActionListener {
		public void actionPerformed(ActionEvent ae) {
			updateView();
		}
	}

	private void castNewFrame() {
		new NewFrame(this);
	}

	private void castChangeFrame() {
		new ChangeFrame(nowPool, this);
	}

	private void castDeleteFrame() {
		new DeleteFrame(this);
	}

	static void setIEditOrder(int iEditOrder) {
		MainFrame.iEditOrder = iEditOrder;
	}

}
