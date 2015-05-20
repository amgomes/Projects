package assign2.weathermap;

import junit.framework.TestCase;
import org.json.JSONException;
import org.mockito.Mockito;

import java.io.IOException;
import java.util.Arrays;
import java.util.List;

import static org.mockito.Mockito.when;

public class WeatherMapTest extends TestCase
{
    private WeatherMap weatherMap;
    private WeatherService weatherService;

    WeatherData sfo = new WeatherData("San Francisco", 59.45, "rainy");
    WeatherData houston = new WeatherData("Houston", 63.19, "sunny");
    WeatherData nyc = new WeatherData("New York", 38.21, "sunny");
    WeatherData chicago = new WeatherData("Chicago", 38.21, "rainy");
    WeatherData miami = new WeatherData("Miami", 79.07, "cloudy");
    WeatherData memphis = new WeatherData("Memphis", 79.07, "cloudy");
    WeatherData memphis1 = new WeatherData("Memphis", 63.19, "sunny");
    WeatherData memphis2 = new WeatherData("Memphis", 63.19, "sunny");
    WeatherData blankCity1 = new WeatherData("", 79.07, "cloudy");
    WeatherData blankCity2 = new WeatherData("", 63.45, "sunny");
    WeatherData blankCity3 = new WeatherData("", 38.21, "sunny");
    WeatherData blankCity4 = new WeatherData("", 59.45, "rainy");
    WeatherData dallas = new WeatherData("Dallas", 79.07, "cloudy");
    WeatherData grr = new WeatherData("Grand Rapids", 0.0, "sunny");
    WeatherData annArbor = new WeatherData("Ann Arbor", -0.0, "sunny");
    WeatherData detroit = new WeatherData("Detroit", 0.000, "cloudy");
    WeatherData denver = new WeatherData("Denver", -0.00, "rainy");

    String serviceFailMessage = "Weather Service Failed.";
    WeatherData errorCity1 = new WeatherData("blah1", new RuntimeException(serviceFailMessage));
    WeatherData errorCity2 = new WeatherData("blah2", new RuntimeException(serviceFailMessage));
    WeatherData errorCity3 = new WeatherData("blah3", new RuntimeException(serviceFailMessage));

    public void testCanary() {
        assertTrue(true);
    }

    public void setUp() {
        weatherMap = new WeatherMap();
        weatherService = Mockito.mock(WeatherService.class);
    }

    public void testSortByCityAlphabetically() {
        List<WeatherData> weatherData = Arrays.asList(sfo, houston, nyc, miami);
        List<WeatherData> sortedWeatherData = Arrays.asList(houston, miami, nyc, sfo);
        assertEquals(sortedWeatherData, weatherMap.sortByCity(weatherData));
    }

    public void testSortingWhenCityNamesAreSameOrderByTemperature() {
        List<WeatherData> weatherData = Arrays.asList(sfo, nyc, memphis, memphis1);
        List<WeatherData> sortedWeatherData = Arrays.asList(memphis1, memphis, nyc, sfo);
        assertEquals(sortedWeatherData, weatherMap.sortByCity(weatherData));
    }

    public void testSortingWhenCityNamesAndTemperatureAreSame() {
        List<WeatherData> weatherData = Arrays.asList(sfo, memphis1, nyc, memphis2);
        List<WeatherData> sortedWeatherData = Arrays.asList(memphis1, memphis2, nyc, sfo);
        assertEquals(sortedWeatherData, weatherMap.sortByCity(weatherData));
    }

    public void testSortByCityForAlreadySortedData() {
        List<WeatherData> weatherData = Arrays.asList(houston, miami, nyc, sfo);
        List<WeatherData> sortedWeatherData = Arrays.asList(houston, miami, nyc, sfo);
        assertEquals(sortedWeatherData, weatherMap.sortByCity(weatherData));
    }

    public void testSortWhenThereIsABlankCity() {
        List<WeatherData> weatherData = Arrays.asList(houston, blankCity1, nyc, sfo);
        List<WeatherData> sortedWeatherData = Arrays.asList(blankCity1, houston, nyc, sfo);
        assertEquals(sortedWeatherData, weatherMap.sortByCity(weatherData));
    }

    public void testWhenThereAreMultipleBlankCitiesSortByCityThenTemperature() {
        List<WeatherData> weatherData = Arrays.asList(houston, blankCity1, nyc, blankCity2);
        List<WeatherData> sortedWeatherData = Arrays.asList(blankCity2, blankCity1, houston, nyc);  
        assertEquals(sortedWeatherData, weatherMap.sortByCity(weatherData));
    }

    public void testWhenThereAreNoCitiesSortByTemperature() {
        List<WeatherData> weatherData = Arrays.asList(blankCity1, blankCity2, blankCity3, blankCity4);
        List<WeatherData> sortedWeatherData = Arrays.asList(blankCity3, blankCity4, blankCity2, blankCity1);
        assertEquals(sortedWeatherData, weatherMap.sortByCity(weatherData));
    }

    public void testGetCityWithHottestTemperature() {
        List<WeatherData> weatherData = Arrays.asList(sfo, houston, nyc, miami);
        List<String> hottestCityExpected = Arrays.asList("Miami");
        List<String> hottestCityResults = weatherMap.getHottestCity(weatherData);
        assertEquals(hottestCityExpected, hottestCityResults);
    }

    public void testTiedHottestCitiesAreReturnedAlphabetically() {
        List<WeatherData> weatherData = Arrays.asList(sfo, miami, nyc, dallas); 
        List<String> hottestCitiesExpected = Arrays.asList("Dallas", "Miami");
        List<String> hottestCitiesResults = weatherMap.getHottestCity(weatherData);
        assertEquals(hottestCitiesExpected, hottestCitiesResults);
    }

    public void testGetCityWithHottestTemperatureWhenThereIsNoData() {
        List<WeatherData> weatherData = Arrays.asList();
        List<String> hottestCityExpected = Arrays.asList();
        List<String> hottestCityResults = weatherMap.getHottestCity(weatherData);
        assertEquals(hottestCityExpected, hottestCityResults);
    }

    public void testGetCityWithColdestTemperature() {
        List<WeatherData> weatherData = Arrays.asList(sfo, houston, nyc, miami);
        List<String> coldestCityExpected = Arrays.asList("New York");
        List<String> coldestCityResults = weatherMap.getColdestCity(weatherData);
        assertEquals(coldestCityExpected, coldestCityResults);
    }

    public void testTiedColdestCitiesAreReturnedAlphabetically() {
        List<WeatherData> weatherData = Arrays.asList(houston, nyc, miami, chicago);
        List<String> coldestCitiesExpected = Arrays.asList("Chicago", "New York");
        List<String> coldestCitiesResults = weatherMap.getColdestCity(weatherData);
        assertEquals(coldestCitiesExpected, coldestCitiesResults);
    }

    public void testGetCityWithColdestTemperatureWhenNoDataExists() {
        List<WeatherData> weatherData = Arrays.asList();
        List<String> coldestCityExpected = Arrays.asList();
        List<String> coldestCityResults = weatherMap.getColdestCity(weatherData);
        assertEquals(coldestCityExpected, coldestCityResults);
    }

    public void testColdestCitesWithNegativeZeroAppearBeforePositiveZero() {
        List<WeatherData> weatherData = Arrays.asList(grr, annArbor, detroit, denver);        
        List<String> coldestCitiesExpected = Arrays.asList("Ann Arbor", "Denver", "Detroit", "Grand Rapids");
        List<String> coldestCitiesResults = weatherMap.getColdestCity(weatherData);
        assertEquals(coldestCitiesExpected, coldestCitiesResults);
    }

    public void testGetWeatherReportReturnsDataForSingleCity() throws IOException, JSONException{
        weatherMap.setServiceProvider(weatherService);
        when(weatherService.getWeatherForCity(houston.getCity())).thenReturn(houston);

        WeatherReport expectedWeatherReport =
                new WeatherReport(Arrays.asList(houston), Arrays.asList(houston.getCity()),
                        Arrays.asList(houston.getCity()), Arrays.asList());

        WeatherReport weatherReport = weatherMap.getWeatherReportForCities(Arrays.asList(houston.getCity()));

        assertEquals(expectedWeatherReport.getSortedCitiesData(), weatherReport.getSortedCitiesData());
        assertEquals(expectedWeatherReport.getHottestCities(), weatherReport.getHottestCities());
        assertEquals(expectedWeatherReport.getColdestCities(), weatherReport.getColdestCities());
    }

    public void testGetWeatherReportReturnsDataForMultipleCities() throws IOException, JSONException{
        weatherMap.setServiceProvider(weatherService);

        when(weatherService.getWeatherForCity(houston.getCity())).thenReturn(houston);
        when(weatherService.getWeatherForCity(detroit.getCity())).thenReturn(detroit);
        when(weatherService.getWeatherForCity(dallas.getCity())).thenReturn(dallas);

        WeatherReport expectedWeatherReport =
                new WeatherReport(Arrays.asList(dallas, detroit, houston), Arrays.asList(dallas.getCity()),
                        Arrays.asList(detroit.getCity()), Arrays.asList());

        WeatherReport weatherReport = weatherMap.getWeatherReportForCities(Arrays.asList(houston.getCity(),
            dallas.getCity(),detroit.getCity()));

        assertEquals(expectedWeatherReport.getSortedCitiesData(), weatherReport.getSortedCitiesData());
        assertEquals(expectedWeatherReport.getHottestCities(), weatherReport.getHottestCities());
        assertEquals(expectedWeatherReport.getColdestCities(), weatherReport.getColdestCities());
    }

    public void testWeatherReportIsEmptyForEmptyListOfInputCities() {
        weatherMap.setServiceProvider(weatherService);
        WeatherReport weatherReport = weatherMap.getWeatherReportForCities(Arrays.asList());

        assertTrue(weatherReport.getSortedCitiesData().isEmpty());
        assertTrue(weatherReport.getHottestCities().isEmpty());
        assertTrue(weatherReport.getColdestCities().isEmpty());
    }

    public void testGetWeatherReportReturnsEmptyListWhenNoThereIsNoServiceProvider(){
        WeatherReport weatherReport = weatherMap.getWeatherReportForCities(Arrays.asList(houston.getCity()));
        assertTrue(weatherReport.getSortedCitiesData().isEmpty());
        assertTrue(weatherReport.getHottestCities().isEmpty());
        assertTrue(weatherReport.getColdestCities().isEmpty());
    }

    public void testTransformCityAndHandleExceptionsFromServiceCatchRuntimeException() throws IOException, JSONException{
        WeatherService weatherServiceWithError = Mockito.mock(WeatherService.class);
        when(weatherServiceWithError.getWeatherForCity(errorCity1.getCity()))
                .thenThrow(new RuntimeException(serviceFailMessage));

        weatherMap.setServiceProvider(weatherServiceWithError);
        WeatherData errorData = weatherMap.transformCityAndHandleExceptionsFromService(errorCity1.getCity());

        assertTrue(errorData.hasException());
    }

   
    public void testGetWeatherReportForOneCityThatDoesNotExist() throws IOException, JSONException{
        WeatherService weatherServiceWithError = Mockito.mock(WeatherService.class);
        when(weatherServiceWithError.getWeatherForCity(errorCity1.getCity())).thenReturn(errorCity1);

        weatherMap.setServiceProvider(weatherServiceWithError);

        WeatherReport expectedWeatherReport = new WeatherReport(Arrays.asList(), Arrays.asList(),
                Arrays.asList(), Arrays.asList(errorCity1));

        WeatherReport weatherReport = weatherMap.getWeatherReportForCities(Arrays.asList(errorCity1.getCity()));

        assertEquals(expectedWeatherReport.getSortedCitiesData(), weatherReport.getSortedCitiesData());
        assertEquals(expectedWeatherReport.getHottestCities(), weatherReport.getHottestCities());
        assertEquals(expectedWeatherReport.getColdestCities(), weatherReport.getColdestCities());
        assertEquals(expectedWeatherReport.getErrorMessages(), weatherReport.getErrorMessages());
    }

    public void testGetWeatherReportForMultipleCitiesWithOneIncorrectCity() throws IOException, JSONException{
        WeatherService weatherServiceWithErr = Mockito.mock(WeatherService.class);
        when(weatherServiceWithErr.getWeatherForCity(houston.getCity())).thenReturn(houston);
        when(weatherServiceWithErr.getWeatherForCity(detroit.getCity())).thenReturn(detroit);
        when(weatherServiceWithErr.getWeatherForCity(errorCity1.getCity())).thenReturn(errorCity1);

        weatherMap.setServiceProvider(weatherServiceWithErr);

        WeatherReport expectedWeatherReport = new WeatherReport(Arrays.asList(detroit, houston),
                Arrays.asList(houston.getCity()), Arrays.asList(detroit.getCity()), Arrays.asList(errorCity1));

        WeatherReport weatherReport = weatherMap.getWeatherReportForCities(Arrays.asList(errorCity1.getCity(),
                houston.getCity(), detroit.getCity()));

        assertEquals(expectedWeatherReport.getSortedCitiesData(), weatherReport.getSortedCitiesData());
        assertEquals(expectedWeatherReport.getHottestCities(), weatherReport.getHottestCities());
        assertEquals(expectedWeatherReport.getColdestCities(), weatherReport.getColdestCities());
        assertEquals(expectedWeatherReport.getErrorMessages(), weatherReport.getErrorMessages());
    }

    public void testWeatherReportWhenServiceFailsForAllCities() throws IOException, JSONException{
        WeatherService weatherServiceWithErr = Mockito.mock(WeatherService.class);
        when(weatherServiceWithErr.getWeatherForCity(errorCity1.getCity())).thenReturn(errorCity1);
        when(weatherServiceWithErr.getWeatherForCity(errorCity2.getCity())).thenReturn(errorCity2);
        when(weatherServiceWithErr.getWeatherForCity(errorCity3.getCity())).thenReturn(errorCity3);

        weatherMap.setServiceProvider(weatherServiceWithErr);

        WeatherReport expectedWeatherReport = new WeatherReport(Arrays.asList(), Arrays.asList(),
                Arrays.asList(), Arrays.asList(errorCity1, errorCity2, errorCity3));

        WeatherReport weatherReport = weatherMap.getWeatherReportForCities(Arrays.asList(errorCity1.getCity(),
                errorCity2.getCity(), errorCity3.getCity()));

        assertEquals(expectedWeatherReport.getSortedCitiesData(), weatherReport.getSortedCitiesData());
        assertEquals(expectedWeatherReport.getHottestCities(), weatherReport.getHottestCities());
        assertEquals(expectedWeatherReport.getColdestCities(), weatherReport.getColdestCities());
        assertEquals(expectedWeatherReport.getErrorMessages(), weatherReport.getErrorMessages());
    }
}
