import { Stack, StackProps } from 'aws-cdk-lib';
import { Construct } from 'constructs';
import { DiscordConfig, DiscordSecret } from './internal/discord-secretes';
// import * as sqs from 'aws-cdk-lib/aws-sqs';

export class DiscordBot extends Stack {
  constructor(scope: Construct, id: string, config: DiscordConfig, props?: StackProps) {
    super(scope, id, props);

    new DiscordSecret(this, `discord-loabot-secret`, {}, config);
  }
}
