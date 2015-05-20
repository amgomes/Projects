package assign3.fibonacci;

import java.util.function.UnaryOperator;
import java.util.stream.Stream;

import static java.util.stream.Collectors.toList;

public class FibonacciIterative implements Fibonacci {
    public long fibonacciValueAt(int position) {
        if (position < 0) return -1;
        if(position < 2) return 1;

        int[] seriesStart = new int[] {1, 1};
        UnaryOperator<int[]> nextInSeries = series -> new int[] {series[1], series[0] + series[1]};

        return Stream.iterate(seriesStart, nextInSeries)
                      .limit(position)
                      .map(series -> series[1])
                      .collect(toList())
                      .get(position - 1);
    }
}