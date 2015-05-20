package assign4.stringwriter;

import java.util.Arrays;
import java.util.stream.Collectors;

public class DuplicateRemover {
    public static String convert(String input) {
        return removeDuplicate(input);
    }

    private static String removeDuplicate(String input) {
        return Arrays.stream(input.split(" "))
                     .distinct()
                     .collect(Collectors.joining(" "));
    }
}
