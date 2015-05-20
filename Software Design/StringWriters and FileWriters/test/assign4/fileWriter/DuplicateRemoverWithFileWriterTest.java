package assign4.fileWriter;

import assign4.stringwriter.StringWriter;
import assign4.writer.DuplicateRemoverWithWriterTest;
import assign4.writer.Writer;

import java.io.File;

public class DuplicateRemoverWithFileWriterTest extends DuplicateRemoverWithWriterTest {
    private String filepath = "testFile.txt";

    public void tearDown() {
        new File(filepath).delete();
    }

    @Override
    public Writer createWriter() {
        return new StringWriter();
    }
}
