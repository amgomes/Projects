package assign4.stringwriter;

import assign4.writer.Writer;

import java.util.function.Function;
import java.util.stream.Stream;

public class StringWriter implements Writer {
    private StringBuilder string = new StringBuilder();
    private Function<String, String> transformer;

    public StringWriter() {
      setTransformer();
    }

    @Override
    public void write(String inputString) {
        string.append(transformer.apply(inputString));
    }

    @Override
    public String read() {
        return string.toString();
    }

    @SafeVarargs
    @Override
    public final void setTransformer(Function<String, String>... transformers) {
        transformer = Stream.of(transformers)
                            .reduce(Function.identity(), Function::andThen);
    }
}