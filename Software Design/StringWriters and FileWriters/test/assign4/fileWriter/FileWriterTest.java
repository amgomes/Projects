package assign4.fileWriter;

import assign4.writer.Writer;
import assign4.writer.WriterTest;
import org.mockito.Mockito;

import java.io.File;
import java.io.IOException;
import java.io.PrintWriter;

import static org.mockito.Mockito.doNothing;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;

public class FileWriterTest extends WriterTest {
    private FileWriter fileWriter;
    private String filepath = "testFile.txt";

    public void setUp() throws IOException{
        super.setUp();
        fileWriter = new FileWriter(filepath);
    }

    @Override
    public Writer createWriter() throws IOException {
        return new FileWriter(filepath);
    }

    public void tearDown() {
        new File(filepath).delete();
    }

    public void testFileWriterWritesInputToValidFile() throws IOException {
        FileWriter spy = Mockito.spy(new FileWriter(filepath));
        PrintWriter mockedWriter = Mockito.mock(PrintWriter.class);
        doNothing().when(mockedWriter).write(Mockito.anyString());
        spy.setPrintWriter(mockedWriter);
        spy.write("Trying to write first time");
        verify(mockedWriter, times(1)).write(Mockito.anyString());
    }

    public void testFileWriterThrowsExceptionForInvalidFile() throws IOException {
        try {
            FileWriter invalidFileWriter = new FileWriter("/blah/blah/file.txt");
            invalidFileWriter.write("Not Working");
            fail("Expected exception for IO operation");
        }
        catch(IOException ex) {
            assertTrue(true);
        }
    }

    public void testFileWriterAppendsInputToExistingFile() throws IOException {
        FileWriter spy = Mockito.spy(new FileWriter(filepath));
        PrintWriter mockedWriter = Mockito.mock(PrintWriter.class);
        doNothing().when(mockedWriter).write(Mockito.anyString());
        spy.setPrintWriter(mockedWriter);
        spy.write("Trying to write first time");
        spy.write("Trying to write second time");
        verify(mockedWriter, times(2)).write(Mockito.anyString());
    }
}
