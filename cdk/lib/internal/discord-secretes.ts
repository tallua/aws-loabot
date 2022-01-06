import { Construct } from 'constructs';
import * as secrets from 'aws-cdk-lib/aws-secretsmanager'

export interface DiscordConfig {
  secret: {
      password: string;
  };
  discord: {
    appid: string;
    appkey: string;
    token: string;
  }
};

export class DiscordSecret extends secrets.Secret {
  constructor(scope: Construct, id: string, props: secrets.SecretProps, config: DiscordConfig) {
    super(scope, id, {
      ...props,
      generateSecretString: {
        generateStringKey: config.secret.password,
        secretStringTemplate: JSON.stringify(config.discord),
      }
    });
  }
}