import { Stack, StackProps } from 'aws-cdk-lib';
import { Construct } from 'constructs';
import { Function, Runtime, Code } from 'aws-cdk-lib/aws-lambda';

export interface HandlerConfig {
  name: string;
  lambda: {
    postfix: string;
    path: string;
  }[];
};

export class DiscordHandler extends Stack {

  constructor(scope: Construct, id: string, config: HandlerConfig, props?: StackProps) {
    super(scope, id, props);

    Object.entries(config.lambda).forEach(([lambda, lambda_info]) => {
      new Function(this, `${id}Lambda${lambda}`, {
        functionName: `${config.name}${lambda_info.postfix}`,
        runtime: Runtime.PROVIDED_AL2,
        handler: 'index.handler',
        code: Code.fromAsset(lambda_info.path)
      });
    });
  }
}
