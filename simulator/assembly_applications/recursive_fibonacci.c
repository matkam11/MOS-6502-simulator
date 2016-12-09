int recursive_fibonacci(int x) {
    if (x == 0)
        return 0;

    if (x == 1)
        return 1;

    return recursive_fibonacci(x-1)+recursive_fibonacci(x-2);
}
