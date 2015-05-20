package assign3.fibonacci;

import junit.framework.TestCase;

public abstract class FibonacciTest extends TestCase {
    private Fibonacci _fibonacci; 

    public void testCanary() {
        assertTrue(true);
    }

    public void setUp() {
        _fibonacci = createFibonacci();
    }

    public abstract Fibonacci createFibonacci();

    public void testIfPositionIsZeroReturnFibonacciOne() {
        assertEquals(1, _fibonacci.fibonacciValueAt(0));
    }

    public void testIfPositionIsOneReturnFibonacciOne() {
        assertEquals(1, _fibonacci.fibonacciValueAt(1));
    }

    public void testIfPositionTwoReturnsCorrectFibonacciNumber() {
        assertEquals(2, _fibonacci.fibonacciValueAt(2));
    }

    public void testValidPositionReturnsCorrectFibonacciNumber() {
        assertEquals(21, _fibonacci.fibonacciValueAt(7));
    }

    public void testIfPositionIsLessThanZeroReturnNegativeOne() {
        assertEquals(-1, _fibonacci.fibonacciValueAt(-10));
    }
}
