package weathermap;

import java.util.Collections;
import java.util.List;

import static java.util.Comparator.comparing;
import static java.util.stream.Collectors.toList;

public class WeatherMap
{
    private WeatherService serviceProvider;

    public List<WeatherData> sortByCity(List<WeatherData> weatherData) {
        return weatherData.stream()
                          .sorted(comparing(WeatherData::getCity).thenComparing(WeatherData::getTemperature))
                          .collect(toList());
    }

    public List<String> getHottestCity(List<WeatherData> weatherData) {
        double hottestTemperature = weatherData.stream()
                                                .mapToDouble(WeatherData::getTemperature)
                                                .max()
                                                .orElse(0.0);

       return weatherData.stream()
                         .filter(data -> data.getTemperature() == hottestTemperature)
                         .map(WeatherData::getCity)
                         .sorted()
                         .collect(toList());
    }

    public List<String> getColdestCity(List<WeatherData> weatherData) {
        double coldestTemperature = weatherData.stream()
                                                .mapToDouble(WeatherData::getTemperature)
                                                .min()
                                                .orElse(0.0);
                     
       return weatherData.stream()
                         .filter(data -> data.getTemperature() == coldestTemperature)
                         .map(WeatherData::getCity)
                         .sorted()
                         .collect(toList());
    }

     public WeatherReport getWeatherReportForCities(List<String> cities) {
        WeatherReport weatherReport = new WeatherReport(Collections.<WeatherData>emptyList(), 
                            Collections.emptyList(), Collections.emptyList(), Collections.emptyList());
        if(serviceProvider != null){
            List<WeatherData> weatherForAllCities = cities.stream()
                                                          .map(this::transformCityAndHandleExceptionsFromService)
                                                          .collect(toList());

            List<WeatherData> weatherForExistingCities = weatherForAllCities.stream()
                                                                            .filter(data -> !data.hasException())
                                                                            .collect(toList());

            List<WeatherData> errorMessages = weatherForAllCities.stream()
                                                                 .filter(data -> data.hasException())
                                                                 .sorted(comparing(WeatherData::getCity))
                                                                 .collect(toList());

            weatherReport = new WeatherReport(sortByCity(weatherForExistingCities), getHottestCity(weatherForExistingCities),
                    getColdestCity(weatherForExistingCities), errorMessages);
        }
        return weatherReport;
    }

     public WeatherData transformCityAndHandleExceptionsFromService(String city){
        try{
            return serviceProvider.getWeatherForCity(city);
        }catch(Exception e){
            return new WeatherData(city, e);
        }
    }

    public void setServiceProvider(WeatherService theServiceProvider) {
        serviceProvider = theServiceProvider;
    }
}
