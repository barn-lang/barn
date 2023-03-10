for file in ./examples/*.ba; do
    echo "Testing file $file"
    ./barn $file
done
