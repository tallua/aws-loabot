#include <iostream>

#include <aws/lambda-runtime/runtime.h>

using namespace std;
using namespace aws::lambda_runtime;

int main(int, char**)
{
    cout << "new lambda launched" << endl;

    aws::lambda_runtime::run_handler([](auto&&) {
        return invocation_response::failure("Not Implemented", "500");
    });

    return 0;
}