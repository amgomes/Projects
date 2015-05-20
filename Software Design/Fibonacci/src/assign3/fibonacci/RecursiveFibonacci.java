package assign3.fibonacci;

public class RecursiveFibonacci implements Fibonacci {
    @Override
    public long fibonacciValueAt(int position) {
        if (position < 0) return -1;
        if (position < 2) return 1;

        return fibonacciValueAt(position - 1) + fibonacciValueAt(position - 2);
    }
}