package weathermap;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLConnection;
import java.net.SocketTimeoutException;

public class OpenWeatherMapService implements WeatherService{

    JSONObject getJSON(String city) throws IOException, JSONException {
        try{
            URLConnection connection = connectToURL(city);
            BufferedReader readBuffer = new BufferedReader(new InputStreamReader(connection.getInputStream()));
            JSONObject cityDetails = new JSONObject(readBufferedReader(readBuffer));
            return cityDetails;
        }catch(SocketTimeoutException e){
            return new JSONObject("{ }");
        }
    }

    URLConnection connectToURL (String city) throws IOException {
        return new URL(formUrl(city)).openConnection();
    }

    String formUrl(String nameOfCity) throws MalformedURLException {
        URL urlWeather;
        if (nameOfCity.matches("[a-zA-Z, ]*")) {
            urlWeather = new URL("http://api.openweathermap.org/data/2.5/weather?q="
                    + nameOfCity.replaceAll("\\s", "") + "&units=imperial");
            return (urlWeather.toString());
        } else
            return "Bad City Name";
    }

    String readBufferedReader(BufferedReader readBuffer) throws IOException {
        StringBuilder jsonText = new StringBuilder();
        String line;
        while ((line = readBuffer.readLine()) != null) {
            jsonText.append(line);
        }
        return jsonText.toString();
    }

    WeatherData parseJSONToGetWeatherDetails(JSONObject jsonObj) throws JSONException {
        JSONArray weatherArr = jsonObj.getJSONArray("weather");

        String condition = weatherArr.getJSONObject(0).getString("main");
        String city = jsonObj.getString("name");
        double temperature = jsonObj.getJSONObject("main").getDouble("temp");

        return new WeatherData(city, temperature, condition);
    }

    @Override
    public WeatherData getWeatherForCity(String city) throws IOException, JSONException {
        JSONObject cityJson = getJSON(city);
        return parseJSONToGetWeatherDetails(cityJson);
    }
}
