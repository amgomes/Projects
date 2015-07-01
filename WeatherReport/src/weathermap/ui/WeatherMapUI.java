package weathermap.ui;

import weathermap.OpenWeatherMapService;
import weathermap.WeatherMap;
import weathermap.WeatherReport;

import java.nio.charset.Charset;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.Arrays;
import java.util.List;
import java.util.stream.Stream;

import static java.util.stream.Collectors.toList;

public class WeatherMapUI {
    public static void main (String args[]) {
        WeatherMap weatherMap = new WeatherMap();
        weatherMap.setServiceProvider(new OpenWeatherMapService());

        List<String> cities = getCitiesFromFile();
        WeatherReport weatherReport = weatherMap.getWeatherReportForCities(cities);
        printWeatherReport(weatherReport);
    }

    static List<String> getCitiesFromFile() {
        try {
            Stream<String> cities = Files.lines(Paths.get("src", "cities.txt"), Charset.defaultCharset());
            return cities.collect(toList());
        }
        catch (Exception e) {
            System.out.println("Exception thrown reading from file.");
            System.out.println(e.toString());
        }
        return Arrays.asList();
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