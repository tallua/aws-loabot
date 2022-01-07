#!/usr/bin/env node
import 'source-map-support/register';
import * as cdk from 'aws-cdk-lib';
import { DiscordRouter, RouterConfig } from '../lib/discord-router';
import { readFileSync } from 'fs'


const botConfig: {
  router: RouterConfig
} = JSON.parse(readFileSync('../config.cdk.json').toString());

const app = new cdk.App();
new DiscordRouter(app, 'DiscordRouter', botConfig.router, {
  env: { account: process.env.CDK_DEFAULT_ACCOUNT, region: process.env.CDK_DEFAULT_REGION }
});

