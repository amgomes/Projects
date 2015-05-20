package assign2.weathermap;

import org.json.JSONException;
import java.io.IOException;

public interface WeatherService {
     WeatherData getWeatherForCity(String city) throws IOException, JSONException;
}
