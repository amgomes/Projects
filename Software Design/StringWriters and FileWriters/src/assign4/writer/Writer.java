package assign4.writer;

import java.io.IOException;
import java.util.function.Function;

public interface Writer {
    @SuppressWarnings("unchecked")
    void setTransformer(Function<String, String>... operators);
    void write(String string) throws IOException;
    String read() throws IOException;
}
