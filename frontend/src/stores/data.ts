import { defineStore } from 'pinia'
import type { Job, User, Application } from '../types'

async function loadJSON<T>(path: string): Promise<T> {
  const res = await fetch(path)
  if (!res.ok) throw new Error(`Failed to load ${path}`)
  return res.json()
}

// Fixed: Converted to Options API to resolve all "Property 'filter' does not exist on type '{ value: ... }'" errors globally.
export const useDataStore = defineStore('data', {
  state: () => ({
    jobs: [] as Job[],
    users: [] as User[],
    applications: [] as Application[],
    loading: false,
    error: null as string | null
  }),
  actions: {
    async loadAll() {
      this.loading = true
      this.error = null
      try {
        const [j, u, a] = await Promise.all([
          loadJSON<Job[]>('/data/jobs.json'),
          loadJSON<User[]>('/data/users.json'),
          loadJSON<Application[]>('/data/applications.json'),
        ])
        this.jobs = j
        this.users = u
        this.applications = a
      } catch (e: any) {
        this.error = e.message
      } finally {
        this.loading = false
      }
    },
    addJob(job: Omit<Job, 'id'>) {
      const id = this.jobs.length ? Math.max(...this.jobs.map(j => j.id)) + 1 : 1
      this.jobs.push({ ...job, id })
    },
    updateJob(updated: Job) {
      const idx = this.jobs.findIndex(j => j.id === updated.id)
      if (idx !== -1) this.jobs[idx] = updated
    },
    removeJob(id: number) {
      this.jobs = this.jobs.filter(j => j.id !== id)
    },
    toggleJobStatus(id: number) {
      const job = this.jobs.find(j => j.id === id)
      if (job) job.isOpen = !job.isOpen
    },
    getJobById(id: number) {
      return this.jobs.find(j => j.id === id) ?? null
    },
    searchJobs(query: string, category: string, remote: boolean | null) {
      const q = query.toLowerCase()
      return this.jobs.filter(j => {
        const matchQuery =
          !q ||
          j.title.toLowerCase().includes(q) ||
          j.company.toLowerCase().includes(q) ||
          j.location.toLowerCase().includes(q) ||
          j.requiredSkills.some(s => s.toLowerCase().includes(q))
        const matchCat = !category || j.category === category
        const matchRemote = remote === null || j.isRemote === remote
        return matchQuery && matchCat && matchRemote
      })
    },
    getMatchingJobsForUser(userSkills: string[]) {
      return this.jobs
        .filter(j => j.isOpen)
        .map(j => {
          const matched = j.requiredSkills.filter(s =>
            userSkills.map(x => x.toLowerCase()).includes(s.toLowerCase()),
          )
          return { job: j, matchScore: matched.length / Math.max(j.requiredSkills.length, 1) }
        })
        .filter(x => x.matchScore > 0)
        .sort((a, b) => b.matchScore - a.matchScore)
    },
    registerUser(user: Omit<User, 'id'>) {
      if (this.users.some(u => u.username === user.username)) {
        throw new Error('Username already exists')
      }
      const id = this.users.length ? Math.max(...this.users.map(u => u.id)) + 1 : 1
      const newUser: User = { ...user, id }
      this.users.push(newUser)
      return newUser
    },
    loginUser(username: string, password: string): User | null {
      return this.users.find(u => u.username === username && u.password === password) ?? null
    },
    updateUser(updated: User) {
      const idx = this.users.findIndex(u => u.id === updated.id)
      if (idx !== -1) this.users[idx] = updated
    },
    submitApplication(app: Omit<Application, 'applicationId'>) {
      const applicationId = this.applications.length
        ? Math.max(...this.applications.map(a => a.applicationId)) + 1
        : 1
      this.applications.push({ ...app, applicationId })
    },
    updateApplicationStatus(applicationId: number, status: Application['status']) {
      const app = this.applications.find(a => a.applicationId === applicationId)
      if (app) app.status = status
    },
    getApplicationsByUser(userId: number) {
      return this.applications.filter(a => a.userId === userId)
    },
    getApplicationsByJob(jobId: number) {
      return this.applications.filter(a => a.jobId === jobId)
    },
    hasApplied(userId: number, jobId: number) {
      return this.applications.some(a => a.userId === userId && a.jobId === jobId)
    }
  }
})