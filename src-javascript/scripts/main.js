'use strict'

import {Client} from './Client.js';

const requests =
{
  getMail: 0,
  getStatus: 1,
  sendMail: 2,
  deleteMail: 3,
}

let currentRequest = null;

document.getElementById('get-mail-button')
    .addEventListener('click', () => {
      setMailIdInputBox(requests.getMail);
    });
document.getElementById('get-status-button')
    .addEventListener('click', () => {
      setMailIdInputBox(requests.getStatus);
    });
document.getElementById('send-mail-button')
    .addEventListener('click', setSendMailInputBoxes);
document.getElementById('delete-mail-button')
    .addEventListener('click', () => {
      setMailIdInputBox(requests.deleteMail);
    });

document.getElementById('send-button')
    .addEventListener('click', () => {
      onSendClicked();
    });

function setMailIdInputBox(newRequest)
{
  document.getElementById("input-area").innerHTML = 
    "<label for=\"email-id-box\">Email id:</label>" +
    "<input type=\"text\" id=\"email-id-box\">";
  
  currentRequest = newRequest;
}

function setSendMailInputBoxes()
{
  document.getElementById("input-area").innerHTML =
  "<fieldset>" +
    "<label for=\"to-email-box\">To E-mail:</label>" +
    "<input type=\"text\" id=\"to-email-box\">" +
    "<label for=\"from-email-box\">From E-mail:</label>" +
    "<input type=\"text\" id=\"from-email-box\">" +
    "<label for=\"subject-box\">Subject:</label>" +
    "<input type=\"text\" id=\"subject-box\">" +
    "<label for=\"body-box\">Body:</label>" +
    "<input type=\"text\" id=\"body-box\">" +
  "</fieldset>";

  currentRequest = requests.sendMail;
}

async function onSendClicked()
{
  const addressInput = document.getElementById("address-box");
  const portInput = document.getElementById("port-box");
  const client = new Client(addressInput.value, portInput.value, "/jsonApi");

  switch(currentRequest)
  {
    case requests.getMail:
    {
      const emailId = document.getElementById("email-id-box");
      
      try
      {
        const jsonReply = await client.getEmail(emailId.value);
        const msg = jsonReply.error ?
            jsonReply.error.message : "Email\n: " + JSON.stringify(jsonReply);
        document.getElementById("result-area").innerHTML = msg;
      }
      catch (error)
      {
        document.getElementById("result-area").innerHTML = error;
      }
      break;
    }
    case requests.getStatus:
    {
      const emailId = document.getElementById("email-id-box");
      try
      {
        const jsonReply = await client.getEmailStatus(emailId.value);
        const msg = jsonReply.error ?
            jsonReply.error.message : "Email status: " + jsonReply.status;
        document.getElementById("result-area").innerHTML = msg;
      }
      catch (error)
      {
        document.getElementById("result-area").innerHTML = error;
      }
      break;
    }
    case requests.sendMail:
    {
      const toEmailInput = document.getElementById("to-email-box");
      const fromEmailInput = document.getElementById("from-email-box");
      const subjectInput = document.getElementById("subject-box");
      const bodyInput = document.getElementById("body-box");

      try
      {
        const jsonReply = await client.sendEmail(
            toEmailInput.value,
            fromEmailInput.value,
            subjectInput.value,
            bodyInput.value);
        
        document.getElementById("result-area").innerHTML = 
            "<div>E-mail Saved with id: " + jsonReply.id + "</div>" +
            "<div id=\"pooling-status\"></div>";
        if(jsonReply.id)
        {
          const intervalId = setInterval(async () => {
            try
            {
              const jsonStatus = await client.getEmailStatus(jsonReply.id);
              document.getElementById("pooling-status").innerHTML = 
                  "E-mail status: " + jsonStatus.status;
              if (jsonStatus.status == "Sent" || jsonStatus.status == "Gave up" 
                  || !jsonStatus.status)
              {
                window.clearInterval(intervalId);
              }
            }
            catch (error)
            {
              window.clearInterval(intervalId);
              document.getElementById("pooling-status").innerHTML = error;
            }
          }, 10000);
        }
      }
      catch (error)
      {
        document.getElementById("result-area").innerHTML = error;
      }
      break;
    }
    case requests.deleteMail:
    {
      const emailId = document.getElementById("email-id-box");
      try
      {
        const jsonReply = await client.deleteEmail(emailId.value);
        const msg = jsonReply.error ? 
            jsonReply.error.message : jsonReply.success;
        document.getElementById("result-area").innerHTML = msg;
      }
      catch (error)
      {
        document.getElementById("result-area").innerHTML = error;
      }
      break;
    }
    default:
      // Do nothing.
  }
}