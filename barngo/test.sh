for file in ./examples/*.ba ./tests/*.ba; do
    echo "Testing file $file"
    if [ $file = "./examples/25-opengl-binding.ba" ]; then
        ./barn $file --cflags "-lGL -lglut" --w-disable-unused
    elif [ $file = "./tests/test-12-extern-functions.ba" ]; then
        ./barn $file --no-stdlib --w-disable-unused
    else
        ./barn $file --w-disable-unused
    fi
done
