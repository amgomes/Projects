package assign4.stringwriter;

public interface StupidConverter {
    static String convert(String input) {
        return input.replaceAll("stupid", "s*****");
    }
}
