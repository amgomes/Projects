package assign3.fibonacci;

public class FibonacciIterativeTest extends FibonacciTest {
    @Override
    public Fibonacci createFibonacci() {
        return new FibonacciIterative();
    }
}
