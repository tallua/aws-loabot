#!/usr/bin/env node
import 'source-map-support/register';
import * as cdk from 'aws-cdk-lib';
import { DiscordBot } from '../lib/discord-bot';
import { readFileSync }from 'fs'

const discordConfig = JSON.parse(readFileSync('bot.config.json').toString());

const app = new cdk.App();
new DiscordBot(app, 'DiscordBot', {
  env: { account: process.env.CDK_DEFAULT_ACCOUNT, region: process.env.CDK_DEFAULT_REGION },
  ...discordConfig
});