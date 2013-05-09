package langunator;

public class IntParseException extends Exception {

	/**
	 * 
	 */
	private static final long serialVersionUID = -3296196691571664426L;
	private final String message;

	IntParseException(String ort, String str) {
		message = "Problem bei " + ort + "...     " + str + " ist keine ganze Zahl!";
	}

	@Override
	public String getMessage() {
		return message;
	}

}
