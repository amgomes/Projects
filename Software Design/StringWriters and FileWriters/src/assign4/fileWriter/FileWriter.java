package assign4.fileWriter;

import assign4.writer.Writer;

import java.io.File;
import java.io.IOException;
import java.io.PrintWriter;
import java.nio.charset.Charset;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.function.Function;
import java.util.stream.Collectors;
import java.util.stream.Stream;

public class FileWriter implements Writer {
    
    private PrintWriter printWriter;
    private String filePath;
    private Function<String, String> transformer;

     public FileWriter(String path) throws IOException{
        filePath = path;
        printWriter = new PrintWriter(new java.io.FileWriter(new File(path)), true);
        setTransformer();
    }

    @Override
    public void write(String inputString) throws IOException {
        printWriter.write(transformer.apply(inputString));
    }

    @Override
    public String read() throws IOException {
        printWriter.close();
        return Files.readAllLines(Paths.get(filePath), Charset.defaultCharset())
                    .stream()
                    .collect(Collectors.joining());
    }

    @Override
    @SafeVarargs
    public final void setTransformer(Function<String, String>... transformers) {
        transformer = Stream.of(transformers)
                .reduce(Function.identity(), Function::andThen);
    }

    void setPrintWriter(PrintWriter newWriter) {
        printWriter = newWriter;
    }
}
