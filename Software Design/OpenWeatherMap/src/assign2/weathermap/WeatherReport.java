package assign2.weathermap;

import java.util.List;

public class WeatherReport {
    private List<WeatherData> sortedCitiesData;
    private List<String> hottestCities;
    private List<String> coldestCities;
    private List<WeatherData> errorMessages;

    public WeatherReport(List<WeatherData> citiesData, List<String> hotCities,
                         List<String> coldCities, List<WeatherData> errors) {
        sortedCitiesData = citiesData;
        hottestCities = hotCities;
        coldestCities = coldCities;
        errorMessages = errors;
    }

    public List<WeatherData> getSortedCitiesData() {
        return sortedCitiesData;
    }

    public List<String> getHottestCities() {
        return hottestCities;
    }

    public List<String> getColdestCities() {
        return coldestCities;
    }

    public List<WeatherData> getErrorMessages() {
        return errorMessages;
    }
}