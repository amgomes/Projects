package weathermap;

import java.util.Optional;

public class WeatherData {
    private final String city;
    private double temperature;
    private String condition;
    private Optional <Exception> exception;

    WeatherData(String theCity, double theTemperature, String theCondition) {
        city = theCity;
        temperature = theTemperature;
        condition = theCondition;
        exception = Optional.empty();
    }

    WeatherData(String theCity, Exception theException) {
        city = theCity;
        exception = Optional.of(theException);
    }

    public String getCity() {
        return city;
    }

    public double getTemperature() {
        return temperature;
    }

    public String getCondition() {
        return condition;
    }

    public boolean hasException() {
        return exception.isPresent();
    }

    public String getExceptionMessage() {
        if (hasException())
            return exception.toString();
        else return "";
    }
}
