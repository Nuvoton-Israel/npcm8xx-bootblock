find ./ -type f -name "*.o" -exec rm -vf {} +
find ./ -type f -name "*.o.d" -exec rm -vf {} +
find ./ -type f -name "*.d" -exec rm -vf {} +

rm -rf ./Images
