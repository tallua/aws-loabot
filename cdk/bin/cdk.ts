#!/usr/bin/env node
import 'source-map-support/register';
import * as cdk from 'aws-cdk-lib';
import { DiscordRouter, RouterConfig } from '../lib/discord-router';
import { DiscordHandler, HandlerConfig } from '../lib/discord-handler';
import { readFileSync } from 'fs'


const cdkConfig: {
  handler: HandlerConfig;
  router: RouterConfig;
} = JSON.parse(readFileSync('../config.cdk.json').toString());

const app = new cdk.App();
new DiscordHandler(app, cdkConfig.handler.name, cdkConfig.handler, {
  env: { account: process.env.CDK_DEFAULT_ACCOUNT, region: process.env.CDK_DEFAULT_REGION }
})
new DiscordRouter(app, cdkConfig.router.name, cdkConfig.router, {
  env: { account: process.env.CDK_DEFAULT_ACCOUNT, region: process.env.CDK_DEFAULT_REGION }
});

