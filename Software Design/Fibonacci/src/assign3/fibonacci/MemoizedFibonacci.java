package assign3.fibonacci;

import java.util.HashMap;
import java.util.Map;

public class MemoizedFibonacci extends RecursiveFibonacci {
    Map<Integer, Long> fibonacciNumberMap = new HashMap<>();

    @Override
    public long fibonacciValueAt(int position) {
        return fibonacciNumberMap.computeIfAbsent(position, super::fibonacciValueAt);
    }
}