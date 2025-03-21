## 2인 멀티플레이 숫자 야구 게임
--------------------
### 숫자 야구 규칙 
1. 3가지 숫자를 제시한다.
2. 제시한 숫자 중 정답인 숫자와 같은 위치에 같은 숫자가 존재한다면 스트라이크 (123 / 145 -> 1S0B)
3. 제시한 숫자 중 정답인 숫자와 같은 위치는 아니지만 같은 숫자가 존재한다면 볼 (123 / 451 -> 0S1B)
4. 제시한 숫자 중 중복이 있는 경우 제출 불가

### 게임 루프
![1](https://github.com/user-attachments/assets/6f34139b-3580-4599-9a47-c031c2439df2)

게임이 시작되면 닉네임을 입력할 수 있습니다.

![2](https://github.com/user-attachments/assets/6a7e7338-ea7d-410f-b71b-f6b34a7ca478)

이름을 입력하면 로비 위젯에 사용자들의 이름과 호스트가 누구인지 표시됩니다.

![3](https://github.com/user-attachments/assets/df12c0df-1c43-41ab-a2fb-d48ddb257a1a)

호스트가 아닌 플레이어는 게임 준비를 해야 호스트가 게임을 시작할 수 있으면 플레이어가 게임 준비를 하면 게임 준비 버튼에 V표시가 생깁니다.

![4](https://github.com/user-attachments/assets/1928cd71-20b6-47d0-a53f-e210dc5e17fb)

게임이 시작되면 게임 위젯이 표시되면 이 위젯에서 게임이 진행됩니다.

![5](https://github.com/user-attachments/assets/a69aa7ff-1b03-44a3-bf8b-85f9f13c4c79)

버튼으로 숫자를 조작하고 제출 버튼을 누르면 제출 버튼이 비활성화 됩니다.

![6](https://github.com/user-attachments/assets/efde6b8e-0935-48e6-99f1-255dff741551)

제출 시간이 마감되면 프로그램이 제출한 숫자를 판정해주고 제출 버튼을 다시 활성화 시켜줍니다.

![7](https://github.com/user-attachments/assets/306e70ae-afb7-4e04-8b47-cc299b3c82f6)

제출 마감 시간까지 제출을 하지 않으면 미제출이라고 표시됩니다.

![8](https://github.com/user-attachments/assets/7798411b-e1c8-4403-8dd1-b78cee8a0ce2)

>***게임의 승패 판정 기준***

무승부 - 같은 턴에 두 플레이어 모두 정답을 제출한 경우

승리 - 같은 턴에 나만 정답을 제출하고 상대 플레이어는 오답을 제출한 경우

패배 - 같은 턴에 상대 플레이어가 정답을 제출하고 나는 오답을 제출한 경우
