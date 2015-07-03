package weathermap.ui;

import weathermap.OpenWeatherMapService;
import weathermap.WeatherMap;
import weathermap.WeatherReport;

import java.io.IOException;
import java.nio.charset.Charset;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.List;
import java.util.stream.Stream;

import static java.util.stream.Collectors.toList;

public class WeatherMapUI {
    public static void main (String args[]) {
        WeatherMap weatherMap = new WeatherMap();
        weatherMap.setServiceProvider(new OpenWeatherMapService());

        try {
            List<String> cities = getCitiesFromFile();
            WeatherReport weatherReport = weatherMap.getWeatherReportForCities(cities);
            printWeatherReport(weatherReport);
        }
        catch (Exception e) {
            System.out.println("There was an error processing request: " + e.getMessage());
        }
    }

    static List<String> getCitiesFromFile() throws IOException {
        Stream<String> cities = Files.lines(Paths.get("src", "cities.txt"), Charset.defaultCharset());
        return cities.collect(toList());
    }

    static void printWeatherReport(WeatherReport weatherReport) {
        System.out.println("Cities sorted alphabetically:");
        weatherReport.getSortedCitiesData().stream().forEach(city ->
                System.out.println(city.getCity() + " " + city.getTemperature() + " " + city.getCondition()));

        System.out.println("\nHottest city:");
        weatherReport.getHottestCities().stream().forEach(System.out::println);

        System.out.println("\nColdest city:");
        weatherReport.getColdestCities().stream().forEach(System.out::println);

        System.out.println("\nCities with errors:");
        weatherReport.getErrorMessages().stream().forEach(city ->
                System.out.println(city.getCity() + ": " + city.getExceptionMessage()));
    }
}