package assign3.fibonacci;

import org.mockito.Mockito;

public class RecursiveFibonacciTest extends FibonacciTest {
    @Override
    public Fibonacci createFibonacci() {
        return new RecursiveFibonacci();
    }

    public void testFibonacciValueAtIsCalledRecursively() {
        RecursiveFibonacci fibonacciSpy = Mockito.spy(new RecursiveFibonacci());
        fibonacciSpy.fibonacciValueAt(7);
        Mockito.verify(fibonacciSpy, Mockito.atLeast(41)).fibonacciValueAt(Mockito.anyInt());
    }
}
