package assign2.weathermap;

import junit.framework.TestCase;
import org.json.JSONException;
import org.json.JSONObject;
import org.mockito.Mockito;

import java.io.*;
import java.net.MalformedURLException;
import java.net.SocketTimeoutException;

import static org.mockito.Mockito.doThrow;

public class OpenWeatherMapServiceTest extends TestCase {
    private OpenWeatherMapService openWeatherMapService = new OpenWeatherMapService();
    private String houston = "Houston";

    public void testGetJSONReturnsValidJSON() throws IOException, JSONException {
        assertTrue(openWeatherMapService.getJSON("Houston").getString("name").equalsIgnoreCase("Houston"));
    }

    public void testGetJSONWithInvalidCity() throws IOException, JSONException {
        assertFalse(openWeatherMapService.getJSON("blah").getString("name").equalsIgnoreCase("blah"));
    }

    public void testGetJSONWithBlankCity() throws IOException, JSONException {
        assertTrue(openWeatherMapService.getJSON("").getLong("cod") == 404);
    }

    public void testGetJSONThrowsMalformedURLException() throws IOException, JSONException {
        try {
            openWeatherMapService.getJSON("\\");
            fail("MalformedURLException expected.");
        }
        catch (MalformedURLException e) {
            assertTrue(true);
        }
    }

    public void testConnectToURLThrowsExceptionIfTheNetworkFails() throws IOException, JSONException{
        OpenWeatherMapService spy = Mockito.spy(new OpenWeatherMapService());

        doThrow(new SocketTimeoutException()).when(spy).connectToURL("Houston");

        try {
            spy.getJSON("Houston");
        }
        catch (SocketTimeoutException e) {
            assertTrue(false);
        }
    }
    
    public void testReadBufferedReaderParsesTheInputStream() throws IOException {
        String sampleString = "coord:{lon:-0.83,lat:48.6},sys:{message:0.5498,country:France}";
        InputStream inputStream = new ByteArrayInputStream(sampleString.getBytes());
        BufferedReader reader = new BufferedReader(new InputStreamReader(inputStream));
        assertEquals(openWeatherMapService.readBufferedReader(reader), sampleString);
    }

       public void testReadBufferedReaderDoesNotParseTheInputStreamWithEmptyString() throws IOException {
        String sampleString = "";
        InputStream inputStream = new ByteArrayInputStream(sampleString.getBytes());
        BufferedReader reader = new BufferedReader(new InputStreamReader(inputStream));
        assertEquals(openWeatherMapService.readBufferedReader(reader), sampleString);
    }

    public void testParseJSONObjReturnsDataForValidCity() throws JSONException, IOException {
        assertEquals(houston, openWeatherMapService.getWeatherForCity("Houston").getCity());
    }

    public void testParseJSONObjReturnsExceptionForInvalidCity() throws JSONException, IOException {
        try{
            String cityName = openWeatherMapService.getWeatherForCity("").getCity();
            fail("Should have thrown JSONException");
        }catch (JSONException ex){
            assertTrue(true);
        }
    }

    public void testParseJSONToGetWeatherDetailsReturnsValidWeatherData() throws IOException, JSONException{
        JSONObject jsonObj = new JSONObject("{\"weather\":[{\"main\":\"Rain\",\"description\":\"moderate rain\"}]," +
                "\"main\":{\"temp\":57.22},\"name\":\"Houston\"}");
        WeatherData houstonData = openWeatherMapService.parseJSONToGetWeatherDetails(jsonObj);
        assertEquals("Houston", houstonData.getCity());
        assertEquals("Rain", houstonData.getCondition());
        assertEquals(57.22, houstonData.getTemperature());
    }

    public void testParseJSONToGetWeatherDetailsThrowsExceptionForMissingData() throws IOException, JSONException{
        JSONObject jsonObj = new JSONObject("{\"weather\":[{\"main\":\"Rain\",\"description\":\"moderate rain\"}]," +
                "\"name\":\"Houston\"}");
        try{
            WeatherData houstonData = openWeatherMapService.parseJSONToGetWeatherDetails(jsonObj);
            fail("Should have thrown JSONException");
        }catch (JSONException ex){
            assertTrue(true);
        }
    }
}