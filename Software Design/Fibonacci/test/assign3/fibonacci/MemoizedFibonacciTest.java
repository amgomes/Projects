package assign3.fibonacci;

public class MemoizedFibonacciTest extends FibonacciTest{
    @Override
    public Fibonacci createFibonacci() {
        return new MemoizedFibonacci();
    }

    public void testMemoizedIsOrderOfMagnitudeFasterThanRecursiveFibonacci() {
        long recursiveTime = getFibonacciExecTime(new RecursiveFibonacci());
        long memoizedTime = getFibonacciExecTime(new MemoizedFibonacci());
        assertTrue(recursiveTime > memoizedTime * 10);
    }

    private long getFibonacciExecTime(Fibonacci fibonacci){
        long startTime = System.currentTimeMillis();
        fibonacci.fibonacciValueAt(40);
        return System.currentTimeMillis() - startTime;
    }
}