import { Stack, StackProps } from 'aws-cdk-lib';
import { Construct } from 'constructs';
import { LambdaIntegration, RestApi, Stage, Deployment } from 'aws-cdk-lib/aws-apigateway';
import { Function, Runtime, Code } from 'aws-cdk-lib/aws-lambda';

export interface RouterConfig {
  path: string[];
  stage: {
    [id: string]: {
      appid: string;
      appkey: string;
      token: string;
      callable: string;
    }
  }
};

export class DiscordRouter extends Stack {
  constructor(scope: Construct, id: string, config: RouterConfig, props?: StackProps) {
    super(scope, id, props);

    const router = new Function(this, `${id}Router`, {
      runtime: Runtime.PROVIDED_AL2,
      handler: 'index.handler',
      code: Code.fromAsset('../build/aws-loabot-router/aws-loabot-router.zip')
    });

    const gateway = new RestApi(this, `${id}GW`, {
      deploy: false
    });
    let res = config.path.reduce((r, p) => r.addResource(p), gateway.root);
    res.addMethod('POST', new LambdaIntegration(router, {
      proxy: true
    }));

    Object.entries(config.stage).forEach(([stage, stagevar]) => {
      new Stage(this, `${id}GW${stage}`, {
        stageName: stage,
        deployment: new Deployment(this, `${id}GW${stage}Dep`, {
          api: gateway
        }),
        variables: {
          'DISCORD_APP_ID': stagevar.appid,
          'DISCORD_APP_KEY': stagevar.appkey,
          'DISCORD_BOT_TOKEN': stagevar.token,
          'DISCORD_ROUTE_TABLE_ARN': stagevar.callable
        }
      });
    })
  }
}
