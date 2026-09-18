// Sample MiniLang program: computes a simple sum and checks a condition
int x = 5;
int y = 10;
int sum = 0;
float ratio = 0.5;
string label = "result";

sum = x + y;

if (sum > 10) {
    print(sum);
} else {
    print(x);
}

while (x < y) {
    x = x + 1;
}

print(ratio);
print(label);
